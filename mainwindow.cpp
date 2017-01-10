#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QTime>
#include <QMessageBox>
#include <QToolTip>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),                    //наследование от класса стандартного класса QMainwindow
    ui(new Ui::MainWindow)                  //создание формы по образцу mainwindow.ui с сохранением в поле ui
                                            //форма состоит из 2 страниц stackedWidget: нулевая - игры, первая - меню
{
    ui->setupUi(this);                      //применить все настройки mainwindow.ui
    connect(ui->pushButtonPlay, SIGNAL(clicked()), this, SLOT(initGame())); //привязка кнопки PLAY к собственному методу initGame
    connect(ui->pushButtonExit, SIGNAL(clicked()), this, SLOT(close()));    //привязка кнопки Exit к стандартному методу close
    game.setLabel(ui->label);               //указание объекта QLabel в который будет вставляться игровая сцена

    timerId = -1;                           //если таймер не запущен, то timerId будем считать -1
    updateTime = 20;                        //обновление каждые 20 милисекунд
}

MainWindow::~MainWindow()
{
    delete ui;
}

//вызывается при нажатии пробела в режиме игры
void MainWindow::start()
{
    ui->label->setFocus();                  //для корректной обработки клавиатуры, необходимо снять фокус со всех возможных кнопок
    timerId = startTimer(updateTime);       //запускаем таймер (каждые updateTime милисекунд срабатывает timerEvent
    time = QTime::currentTime();            //запоминаем время
    QToolTip::hideText();                   //убираем подсказку о клавишах
}

//вызывается при нажатии esc в режиме игры или при окончании игры
void MainWindow::stop()
{
    killTimer(timerId);                     //выключаем таймер
    timerId = OFF;                          //сбрасываем timerId
}

//[СИГНАЛ-СЛОТ]вызывается каждые updateTime милисекунд (при включенном таймере)
void MainWindow::timerEvent(QTimerEvent *)
{
    ui->lcdNumberTime->display(time.secsTo(QTime::currentTime()));  //отображаем текущее время
    int res = game.step();                                          //далаем шаг игры (запоминаем результат шага)
    game.draw();                                                    //рисуем игровую сцену

    if (res == Game::GETONE)                                        //результат шага: сбит один шар
        ui->lcdNumberPoints->display(ui->lcdNumberPoints->value() + 1);     //увеличиваем значение в окне отображения на 1

    if (res == Game::WIN || res == Game::LOSE)                      //результат шага: проиграна или выиграна игра
    {
        stop();                                                     //останавливаем игру

        QString text;                                               //текст сообщаения в зависимости от итога
        if (res == Game::WIN)
            text = "CONGRATULATTIONS! YOU WIN!";
        if (res == Game::LOSE)
            text = "SORRY! YOU LOSE!";
        if (QMessageBox::question(this, "END OF GAME!", text + "\n Do you want to restart?") == QMessageBox::Yes)
            initGame();                                             //сбрасываем параметры игры
        else
            ui->stackedWidget->setCurrentIndex(1);                  //переключаемся в режим МЕНЮ
    }
}

//[СИГНАЛ-СЛОТ]вызывается при любом нажатии клавиши
void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Space)                                 //пробел
    {
        if (timerId == OFF)                                         //если игра еще не стартовала
            start();                                                //начинаем игру
    }
    else if (ev->key() == Qt::Key_Escape)                           //esc
    {
        if (timerId == OFF)                                         //если игра еще не стартовала
            ui->stackedWidget->setCurrentIndex(1);                  //переключаемся в режим МЕНЮ
        else
        {
            stop();                                                 //иначем останавливаем игру
            initGame();                                             //сбрасываем параметры игры
        }
    }
    else
        game.setKey(ev->key());                                     //передаем управление в игру для других клавиш
}

//[СИГНАЛ-СЛОТ]вызывается при любом нажатии клавиши
void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Left || ev->key() == Qt::Key_Right)    //при долгом нажатой клавише влево или вправо
        game.setKey(Qt::Key_Up);                                    //необходимо продолжать движение, по клавиша не отожмется
}

//вызывается при нажатии клавиши esc или при окончании игры
void MainWindow::initGame()
{
    game.initBalls(ui->spinBoxAims->value());                       //задаем число шаров (первый spinbox в МЕНЮ)
    game.initTau(ui->spinBoxSpeed->value());                        //задаем скорость (первый spinbox в МЕНЮ)
    game.draw();                                                    //рисуем сцену на ui->label
    ui->lcdNumberPoints->display(0);                                //обнуляем счетчик сбитых шаров (в ИГРЕ)
    ui->stackedWidget->setCurrentIndex(0);                          //переключаемся в режим ИГРЫ
    QToolTip::setFont(QFont("Arial", 16));                          //подсказка об управлении
    QToolTip::showText(QPoint(0,0),
                       "Moving:\tLEFT and RIGHT array \nStart:\tSPACE \nRestart:\tESCAPE");
}

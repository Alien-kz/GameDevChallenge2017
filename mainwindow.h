#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include "game.h"

//MainWindow - интерфейс (вывод изображения, таймер для анимации и работа с клавиатурой)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    //блок для анимации
    enum {ON = 0, OFF = -1};    //режим (ON - анимация, OFF - нет анимации)
    int timerId;                //храним идентификатор таймера (можно запускать несколько таймеров)
    int updateTime;             //время обновления в милисекундах

    //игровой компнент
    Game game;                  //класс с описанием логики игры
    QTime time;                 //время старта игры (для счета секунд)

public:
    MainWindow(QWidget *parent = 0);    //конструктор (настройки при первом запуске приложения)
    ~MainWindow();                      //дестуктор (очистка памяти)

private slots:
    void start();                       //начать игру
    void stop();                        //остановить игру
    void initGame();                    //установить настройки игры
    void timerEvent(QTimerEvent *ev);   //перегруженный метод обработки таймер (каждые updateTime милисекунд)
    void keyPressEvent(QKeyEvent *ev);  //перегруженный метод нажатия на клавиатуру
    void keyReleaseEvent(QKeyEvent *ev);    //перегруженный метод отжатия клавиатуры

private:
    Ui::MainWindow *ui;         //форма с элементами (настраивается в редакторе форм изменением mainwindow.ui)
};

#endif // MAINWINDOW_H

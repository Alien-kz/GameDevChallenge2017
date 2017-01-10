#include "game.h"

#include <QVector2D>
#include <QPainter>
#include <QPen>
#include <QTime>
#include <QSound>
#include <QDebug>
#include <math.h>

//конструктор (вызывает один раз во время запуска всего приложения, как поле mainwindow)
Game::Game()
{
    loadTextures();                         //подгрузка текстур
}

//вызывается в конструкторе Game()
void Game::loadTextures()
{
    backgroundPixmap = new QPixmap(":/pictures/background.png"); //загрузка изображения
    bubblePixmap = new QPixmap(":/pictures/bubble.png");         //если изображения нет - проект не собирается
    ballPixmap = new QPixmap(":/pictures/ball.png");             // адрес относительно
    blockPixmap = new QPixmap(":/pictures/block.png");           // самого файла ресурсов resource.qrc

    pixSize = backgroundPixmap->size();             //границы изображения (нужны для обсчета физики)
    fullScene = new QPixmap(pixSize);               //сцена под размеру изображения
    pixSize.setHeight(pixSize.height() - 10);       //оставляем 10 пикселей под неигровой пол
    radius = ballPixmap->width() / 2;               //радиус шара определяется из размера текстуры
}

Game::~Game()
{
    delete fullScene;
    delete ballPixmap;
    delete blockPixmap;
    delete backgroundPixmap;
    delete bubblePixmap;
}

//инициализация шаров и блока
void Game::initBalls(int n)
{
    block.size = blockPixmap->size();               //определяем размеры блока из размера текстуры
    block.position = fullScene->width() / 2;        //положение - в центре
    block.velocity = 0;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));  //для качественного генераторы случайных чисел

    QVector2D position = QVector2D(pixSize.width() / 2, pixSize.height() - radius - block.size.height()); //центр шара = отступить на блок и радиус шара от нижнего края
    QVector2D velocity = QVector2D(-1 + 2 * qrand() % 10 / 10.0, 1); //случайный вектор скорости (направлен в блок)

    balls.clear();                                  //чистим список шаров
    balls.append(Ball(position, velocity));         //добавляем подвижный шар

    int h, w;                                       //расположение шаров в виде сетки h х w
    for (h = sqrt(n); h > 1; h--)
        if (n % h == 0)
            break;
    w = n / h;                                      //определяем количество узлов в равномерной сетки

    int dy = pixSize.height() / h / 4;              //определяем расстояния между шарами
    int dx = pixSize.width() / w / 2;
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
        {
            QVector2D position = QVector2D(dx * (2 * j + 1), dy * (2 * i + 1));
            QVector2D velocity = QVector2D(0, 0);
            balls.append(Ball(position, velocity)); //добавляем в список неподвижжные шары
        }
}

//вызывается из mainwindow при старте игры
void Game::initTau(int t)
{
    tau = t;
}

//вызывается в конструкторе MainWindow ( game.setLabel(ui->label); )
void Game::setLabel(QLabel *label)
{
    Game::label = label;                        //запоминаем указатель на полотно QLabel, в которое будет вставляться итоговая сцена fullScene
}

//шаг подвижного шара (вызывается в step)
int Game::stepForBall(Ball &b)
{
    b.position = b.position + tau * b.velocity; //дискретное перемещение dx = dt * V

    QVector2D touch = b.position;               //запоминаем точку касания шара стенки
    QVector2D norm;                             //нормаль относительно которой надо отризить

    if (b.position.x() < radius)                //левая стенка
    {
        touch.setX(radius);
        norm = QVector2D(1, 0);
    }
    if (b.position.x() > pixSize.width() - radius)  //правая стенка
    {
        touch.setX(pixSize.width() - radius);
        norm = QVector2D(1, 0);
    }
    if (b.position.y() < radius)                //потолок (координаты отрисовка оси OY сверху вниз)
    {
        touch.setY(radius);
        norm = QVector2D(0, 1);
    }
    if (b.position.y() > pixSize.height() - block.size.height() - radius)   //проверка на проигрыш
    {
        if (b.position.x() < block.position - block.size.width()/2 ||
            b.position.x() > block.position + block.size.width()/2)
            return LOSE;
        touch.setY(pixSize.height() - block.size.height() - radius);
        norm = QVector2D(0, 1);
    }

    if (b.position != touch)                    //если произошло какое-либо касание со стенками или блоком
    {
        b.position = 2 * touch - b.position;    //отразить симместрично touch
        b.velocity = b.velocity - 2 * QVector2D::dotProduct(norm, b.velocity) * norm;   //отразить скорость (упругий удар)
    }
    return NONE;
}

//шаг подвижного шара (вызывается в step)
void Game::stepForBlock(Block &b)
{
    b.position = b.position + 2 * tau * b.velocity; //дискретное перемещение dx = dt * V

    double touch = b.position;
    if (b.position < b.size.width() / 2)            //проверка на пересечение с левой границей
        touch = b.size.width() / 2;
    if (b.position > pixSize.width() - b.size.width() / 2)  //проверка на пересечение с правой границей
        touch = pixSize.width() - b.size.width() / 2;

    if (b.position != touch)                        //если произошло пересечение
    {
        b.position = 2 * touch - b.position;        //отразить положение
        b.velocity = 0;                             //обнулить скорость
    }
}

//шаг подвижного шара (вызывается в step для каждой пары подвижный-неподвижные)
bool Game::checkCollision(Ball &a, const Ball &b)
{
    QVector2D e1(a.position - b.position);          //направление вдоль центров
    if (e1.length() <= 2 * radius)                  //если шары пересекаются
    {
        e1.normalize();                             //ортонормированный базис e1
        QVector2D e2(e1.y(), -e1.x());              //ортонормированный базис e2
        QVector2D velA1 = QVector2D::dotProduct(a.velocity, e1) * e1;   //раскладываем по базису
        QVector2D velA2 = QVector2D::dotProduct(a.velocity, e2) * e2;   //вектор скорости
        a.velocity = -velA1 + velA2;                //отражаем компоненту идущую вдоль центров (упругий удар)
        return true;
    }
    return false;                                   //результат true|false нужен для подсчета очков
}

//вызывается из mainwindow в timerEvent(QTimerEvent *)
int Game::step()
{
    stepForBlock(block);                            //двигаем блок
    if (stepForBall(balls[0]) == LOSE)              //двигаем шар (если проиграли то выходим)
        return LOSE;

    for (int i = 1; i < balls.size(); i++)          //проверяем столкновения с шарами
        if (checkCollision(balls[0], balls[i]))
        {
            balls.removeAt(i);                      //при столкновении удаляем из списка соответстующий шар
            QSound::play(":/sounds/boom.wav");
            return GETONE;
        }
    if (balls.size() == 1)                          //если остался только один шар - выигрыш
        return WIN;

    return NONE;
}

//вызывается из mainwindow в timerEvent(QTimerEvent *)
void Game::draw()
{
    QPainter painter(fullScene);                            //инструмент для рисования на PixMap (и не только)
    painter.drawPixmap(0, 0, *backgroundPixmap);            //фон

    painter.drawPixmap(block.position - block.size.width() / 2,
                       pixSize.height() - block.size.height(),
                       *blockPixmap);                           //блок
    painter.drawPixmap(balls[0].position.x() - radius,
                       balls[0].position.y() - radius,
                       *ballPixmap);                        //подвижный шар

    for (int i = 1; i < balls.size(); i++)                  //неподвижные шары
        painter.drawPixmap(balls[i].position.x() - radius,
                           balls[i].position.y() - radius,
                           *bubblePixmap);
    label->setPixmap(*fullScene);                           //вставки рисунка в полотно QLabel
}

//обработка клавиатуры (не рекомендуется здесь делать какие-либо рассчеты - только запоминать нажатую клавишу)
void Game::setKey(int key)
{
    if (key == Qt::Key_Left)
        block.velocity = -1;
    if (key == Qt::Key_Right)
        block.velocity = 1;
    if (key == Qt::Key_Up)
        block.velocity = 0;
}

#ifndef GAME_H
#define GAME_H

#include <QVector2D>
#include <QLabel>

//Game - игровая логика, физика
//Ball, Block - вспомогательные структуры

//мячи, которые будут сбиваться и который будет сбивать
struct Ball
{
    QVector2D position;     //позиция (класс QVector2D поддерживает операции с двумерными векторами)
    QVector2D velocity;     //скорость
    Ball(const QVector2D & position, const QVector2D & velocity)    //конструктор (для более простого кода генерации объектов)
    {
        Ball::position = position;
        Ball::velocity = velocity;
    }
};

//блок, который отбивает мячи
struct Block
{
    double position;
    double velocity;
    QSize size;
};

//
class Game
{
    //текстуры (рекомендуется создать файл ресурсов и добавлять изображения и звуки как ресурсы)
    QPixmap * backgroundPixmap; //фон - загружается loadTextures()
    QPixmap * bubblePixmap;     //неподвижные шарики - загружается одна текстура в loadTextures()
    QPixmap * ballPixmap;       //подвижный шарик - загружается loadTextures()
    QPixmap * blockPixmap;      //блок - загружается loadTextures()
    QPixmap * fullScene;        //заполняется в draw()

    QSize pixSize;              //размеры сцены
    QLabel *label;              //указатель на полотно QLabel, в которое будет вставляться итоговая сцена fullScene

    int radius;                 //радиус всех шаров
    QList <Ball> balls;         //динамический список шаров (удобно удалять взорванные)
    Block block;                //блок, которые отбивает шары

    double tau;                 //параметр, пропорциональный скорости перемещения всех объектов

    void loadTextures();            //разовая загрузка всех текстур
    void stepForBlock(Block & b);   //сдвиг блока (проверка на границы экрана)
    int stepForBall(Ball & b);      //сдвиг подвижного шара (проверка на границы и проигрыш)
    bool checkCollision(Ball & a, const Ball & b);  //отскок подвижного от неподвижного шара

public:
    Game();
    ~Game();
    int step();                 //один дискретный шаг времени
    void initBalls(int n);      //генерация шариков
    void initTau(int t);        //определение скорости движения объектов
    void setLabel(QLabel *label);   //определение полотна QLabel для вставки сцены
    void setKey(int key);           //определение клавиш, отфильтрованных в mainwindow.cpp

    enum {WIN, LOSE, GETONE, NONE}; //вердикты игры
public slots:
    void draw();                //отрисовка
};

#endif // GAME_H

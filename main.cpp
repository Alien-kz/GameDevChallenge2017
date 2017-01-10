#include "mainwindow.h"
#include <QApplication>

//СТРУКТУРА
//класс MainWindow (наследуется от QMainWindow)- интерфейс (вывод изображения, таймер для анимации, работа с клавиатурой, счёт)
//класс Game - игровая логика, физика
//mainwindow.ui - описание графического интерфейса
//resource.qrc - файл ресурсов (для изображений)
//arcanoid.pro - основной файл проекта

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

# GameDevChallenge2017
GameDevChallenge 2017 in Kaz Branch of MSU

QT [автор Баев А.Ж.]:

0) arcanoid.pro
основной файл проекта QT

1) main.cpp 
создание основного приложения QT с графическим интерфейсом

2) mainwindow.cpp + mainwindow.h:
класс MainWindow (наследуется от QMainWindow)
вывод изображения, таймер для анимации, работа с клавиатурой, счёт

3) mainwindow.ui
ui интерфейс

4) game.cpp + game.h
класс Game
игровая логика, физика

5) resource.qrc
файл ресурсов (для изображений) с ссылками на изображения (pictures/) и звуки (sounds/)

VS + XNA [автор Абдикалыков А.К.]:

Каждый файл .cs - описание класса.
1) Program.cs
Запускает игру

2) Game1.cs
Сама игра: инициализация графических объектов и глобальных переменных, создание и отрисовка каждого отдельного кадра.

3) Ball.cs
Алгоритм поведения мяча

4) Bat.cs
Алгоритм поведения бит

5) Content
Папка, содержащая контент: в данном случае текстуры и шрифт для вывода надписей.

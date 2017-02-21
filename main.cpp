#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.Run();
    qDebug() << "Got here!";
    w.close();


    QApplication::quit();

    //return a.exec();
    return 0;
}

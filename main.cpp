#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;
    w.applicationIcon = QIcon(":/images/rubberducky.jpg");
    w.Run();
    w.close();


    QApplication::quit();

    //return a.exec();
    return 0;
}

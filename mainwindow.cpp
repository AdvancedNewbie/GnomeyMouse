#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "SendKeys.h"

#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createTrayIcon();

    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayClicked(QSystemTrayIcon::ActivationReason)));

}

void MainWindow::sleep(int ms){
#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

MainWindow::~MainWindow()
{
    delete trayIcon;
    delete trayIconMenu;
    delete showAction;
    delete quitAction;
    delete ui;
}

void MainWindow::createActions(){
    showAction = new QAction(tr("&Show"), this);
    connect(showAction, SIGNAL(triggered()), this, SLOT(showThisWindow()));
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quitThisApp()));

}

void MainWindow::showThisWindow(){
    this->show();
}

void MainWindow::quitThisApp(){
    isRunning = false;
}

void MainWindow::Run(){
    QApplication::processEvents();
    mouse myMouse;
    CSendKeys sk;
    QString keys = "@{TAB}";

    ui->xSpin->setValue(trigX);
    ui->ySpin->setValue(trigY);
    QTimer *timer = new QTimer(this);
    timer->start(delay);


    while(isRunning){
        myMouse.x = QCursor::pos().x();
        myMouse.y = QCursor::pos().y();

        //qDebug() << QCursor::pos().x();
        //qDebug() << QCursor::pos().y();

        ui->xCurrent->setText(QString::number(myMouse.x));
        ui->yCurrent->setText(QString::number(myMouse.y));
        qApp->processEvents();

        if (myMouse.x == trigX && myMouse.y == trigY){
            if(timer->remainingTime() <= 0){
                qDebug() << "GNOMEYMOUSE!!!!!";
                sk.SendKeys(keys.toStdString().c_str());
                QTimer *procTimer = new QTimer(this);
                procTimer->start(1000);
                while(procTimer->remainingTime() > 0){
                    qApp->processEvents();
                }
                delete procTimer;
            }
        } else {
            timer->setInterval(delay/3);
        }

        //sleep(1000);
        //qDebug() << timer->remainingTime();
    }
    delete timer;
    QApplication::quit();
}

void MainWindow::createTrayIcon(){
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(showAction);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    QIcon icon("C:\\Users\\unknown\\Pictures\\rubberducky.jpg");
    trayIcon->setIcon(icon);
}

void MainWindow::trayClicked(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        qDebug() << "Trigger!";
        break;
    case QSystemTrayIcon::DoubleClick:
        qDebug() << "DC!";
        this->show();
        break;
    case QSystemTrayIcon::MiddleClick:
        qDebug() << "MC!";
        break;
    default:
        ;
    }
}

void MainWindow::on_setPush_clicked()
{
     trigX = ui->xSpin->value();
     trigY = ui->ySpin->value();

     delay = ui->delaySpin->value();
}

void MainWindow::on_actionQuit_triggered()
{
    isRunning = false;
}

void MainWindow::on_actionMinimize_to_tray_triggered()
{
    this->hide();
}

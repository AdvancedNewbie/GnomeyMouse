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

    settings = new QSettings(qApp->applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    qDebug() << "Settings.ini Path: " << qApp->applicationDirPath() + "/settings.ini";
    QFileInfo checkFile(qApp->applicationDirPath() + "/settings.ini");
    if (checkFile.exists() && checkFile.isFile()){
        qDebug() << "'settings.ini' found!";
        QStringList regList = settings->childGroups();
        qDebug() << "\tregList Child Groups: " << regList << " regList Length: " << regList.length();
        if (regList.length() > 0){
            if(regList.contains("Trigger")){
                qDebug() << "Found 'Trigger' group in settings.ini...";
                settings->beginGroup("Trigger");
                QStringList trigList = settings->childKeys();
                qDebug() << "\ttrigList Child Groups: " << trigList << " trigList Length: " << trigList.length();
                if (trigList.contains("trigX")){
                    trigX = settings->value("trigX").toInt();
                    ui->xSpin->setValue(trigX);
                    qDebug() << "Set 'trigX' to: " << trigX;
                }
                if (trigList.contains("trigY")){
                    trigY = settings->value("trigY").toInt();
                    ui->ySpin->setValue(trigY);
                    qDebug() << "Set 'trigY' to: " << trigY;
                }
                settings->endGroup();
            }

            if(regList.contains("Delay")){
                qDebug() << "Found 'Delay' group in settings.ini...";
                settings->beginGroup("Delay");
                QStringList delayList = settings->childKeys();
                qDebug() << "\tdelayList Child Groups: " << delayList << " delayList Length: " << delayList.length();
                if (delayList.contains("delayBefore")){
                    delayBefore = settings->value("delayBefore").toInt();
                    ui->delayBeforeSpin->setValue(delayBefore);
                    qDebug() << "Set 'delayBefore' to: " << delayBefore;
                }
                if (delayList.contains("delayAfter")){
                    delayAfter = settings->value("delayAfter").toInt();
                    ui->delayAfterSpin->setValue(delayAfter);
                    qDebug() << "Set 'delayAfter' to: " << delayAfter;
                }
                settings->endGroup();
            }

            if(regList.contains("Leeway")){
                qDebug() << "Found 'Leeway' group in settings.ini...";
                settings->beginGroup("Leeway");
                QStringList LeewayList = settings->childKeys();
                qDebug() << "\tLeewayList Child Groups: " << LeewayList << " LeewayList Length: " << LeewayList.length();
                if (LeewayList.contains("PixelLeeway")){
                    PixelLeeway = settings->value("PixelLeeway").toInt();
                    ui->leewaySpin->setValue(PixelLeeway);
                    qDebug() << "Set 'PixelLeeway' to: " << PixelLeeway;
                }
                settings->endGroup();
            }
        }
    } else {
        QFile newSettings(qApp->applicationDirPath() + "/settings.ini");
        newSettings.open(QIODevice::ReadWrite);
        newSettings.close();
        qDebug() << "'settings.ini' did not exist, so I created it.";
        //QRect rec = QApplication::desktop()->screenGeometry();
        //trigX = rec.width();
        //trigY = rec.height();

        trigX = 0;
        trigY = 0;
    }


    ui->setPush->setEnabled(false);
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
    trayIcon->hide();
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
    //Setup the Tray Icon

    setWindowIcon(applicationIcon);


    createActions();
    createTrayIcon();

    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayClicked(QSystemTrayIcon::ActivationReason)));

    QApplication::processEvents();

    CSendKeys sk;
    QString keys = "@{TAB}";

    ui->xSpin->setValue(trigX);
    ui->ySpin->setValue(trigY);
    qint64 beforeTime = QDateTime::currentMSecsSinceEpoch();
    qint64 currentTime = beforeTime;

    //Continuously check Mouse Coordinates for a match to Trigger Coordinates
    while(isRunning){
        myMouse.x = QCursor::pos().x();
        myMouse.y = QCursor::pos().y();

        //qDebug() << QCursor::pos().x();
        //qDebug() << QCursor::pos().y();

        ui->xCurrent->setText(QString::number(myMouse.x));
        ui->yCurrent->setText(QString::number(myMouse.y));
        qApp->processEvents();

        //x in between trigx+ trigx-
        //gt -
        //lt +
        if ((myMouse.x > (trigX-PixelLeeway)) && (myMouse.x < (trigX+PixelLeeway)) && (myMouse.y > (trigY-PixelLeeway)) && (myMouse.y < (trigY+PixelLeeway))){
            currentTime = QDateTime::currentMSecsSinceEpoch();
            if((currentTime - beforeTime) >= delayBefore){
                qDebug() << "GNOMEYMOUSE!!!!!";
                sk.SendKeys(keys.toStdString().c_str());  //Send the Windows + TAB keys to do Task View
                currentTime = QDateTime::currentMSecsSinceEpoch();
                while((QDateTime::currentMSecsSinceEpoch() - currentTime) <= delayAfter){
                    qApp->processEvents();
                }
            }
        } else {

            beforeTime = QDateTime::currentMSecsSinceEpoch();
        }

        //sleep(1000);
        //qDebug() << timer->remainingTime();
    }

    QApplication::quit();
}

void MainWindow::createTrayIcon(){
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(showAction);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    //QIcon icon(":icon/rubberducky.ico");
    trayIcon->setIcon(applicationIcon);
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

     PixelLeeway = ui->leewaySpin->value();

     delayBefore = ui->delayBeforeSpin->value();
     delayAfter = ui->delayAfterSpin->value();

     ui->setPush->setEnabled(false);

     settings->beginGroup("Trigger");
     settings->setValue("trigX", trigX);
     settings->setValue("trigY", trigY);
     settings->endGroup();
     settings->beginGroup("Leeway");
     settings->setValue("PixelLeeway", PixelLeeway);
     settings->endGroup();
     settings->beginGroup("Delay");
     settings->setValue("delayBefore", delayBefore);
     settings->setValue("delayAfter", delayAfter);
     settings->endGroup();

     qDebug() << "Updated settings.";
}

void MainWindow::on_actionQuit_triggered()
{
    isRunning = false;
}

void MainWindow::on_actionMinimize_to_tray_triggered()
{
    this->hide();
}

void MainWindow::on_autoMousePush_clicked()
{
    QString buttonText = ui->autoMousePush->text();
    int count = 400;
    while(count >= 0){
        myMouse.x = QCursor::pos().x();
        myMouse.y = QCursor::pos().y();
        ui->xCurrent->setText(QString::number(myMouse.x));
        ui->yCurrent->setText(QString::number(myMouse.y));
        ui->autoMousePush->setText(QString::number((count--)/100));
        QApplication::processEvents();
        Sleep(9);
    }

    ui->xSpin->setValue(myMouse.x);
    ui->ySpin->setValue(myMouse.y);

    ui->autoMousePush->setText(buttonText);
    ui->setPush->setEnabled(true);

}

void MainWindow::on_actionAbout_triggered()
{
    about.show();
}



void MainWindow::on_xSpin_valueChanged(int arg1)
{
    ui->setPush->setEnabled(true);
}

void MainWindow::on_ySpin_valueChanged(int arg1)
{
    ui->setPush->setEnabled(true);
}

void MainWindow::on_delayBeforeSpin_valueChanged(int arg1)
{
    ui->setPush->setEnabled(true);
}

void MainWindow::on_delayAfterSpin_valueChanged(int arg1)
{
    ui->setPush->setEnabled(true);
}

void MainWindow::on_leewaySpin_valueChanged(int arg1)
{
    ui->setPush->setEnabled(true);
}

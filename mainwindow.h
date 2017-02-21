#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include <QDebug>
#include <iostream>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QSettings>
#include <QFileInfo>
#include <QFile>
#include <QDateTime>
#include "aboutdialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Run();
    QIcon applicationIcon;
    void createActions();
    void createTrayIcon();


private:
    Ui::MainWindow *ui;
    void sleep(int ms);
    struct mouse {
        int x;
        int y;
    };
    mouse myMouse;


    QAction *quitAction;
    QAction *showAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    bool isRunning = true;
    int trigX = -352;
    int trigY = -1080;
    int PixelLeeway = 1; //ms
    int delayBefore = 500; //ms
    int delayAfter = 3000; //ms

    QSettings* settings;

    aboutDialog about;




private slots:
    void trayClicked(QSystemTrayIcon::ActivationReason reason);
    void showThisWindow();
    void quitThisApp();

    void on_setPush_clicked();
    void on_actionQuit_triggered();
    void on_actionMinimize_to_tray_triggered();

    //void closeEvent (QCloseEvent *event);
    void on_autoMousePush_clicked();

    void on_xSpin_valueChanged(int arg1);
    void on_ySpin_valueChanged(int arg1);
    void on_delayBeforeSpin_valueChanged(int arg1);
    void on_actionAbout_triggered();
    void on_delayAfterSpin_valueChanged(int arg1);
    void on_leewaySpin_valueChanged(int arg1);
};

#endif // MAINWINDOW_H

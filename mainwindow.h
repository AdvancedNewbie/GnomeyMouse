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


private:
    Ui::MainWindow *ui;
    void sleep(int ms);
    struct mouse {
        int x;
        int y;
    };
    void createActions();
    void createTrayIcon();

    QAction *quitAction;
    QAction *showAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    bool isRunning = true;
    int trigX = -352;
    int trigY = -1080;
    int delay = 500; //ms


private slots:
    void trayClicked(QSystemTrayIcon::ActivationReason reason);
    void showThisWindow();
    void quitThisApp();

    void on_setPush_clicked();
    void on_actionQuit_triggered();
    void on_actionMinimize_to_tray_triggered();

    //void closeEvent (QCloseEvent *event);
};

#endif // MAINWINDOW_H

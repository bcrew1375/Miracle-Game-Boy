#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>
#include <QSettings>
#include <QMainWindow>

#include "platform.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private:
        Ui::MainWindow *ui;

        Platform *platform;

    private slots:
        void on_actionOpen_triggered();

        void on_actionExit_triggered();

        void on_actionRun_triggered();

        void on_actionPause_triggered();

        void statusBarUpdate();

};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "platform.h"
#include "opengl/opengl.h"
#include "ui_mainwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    private:
        OpenGlWidget* openGlWidget;

        Platform* platform;

        QTimer* statusBarTimer;

        Ui::MainWindow* ui;

    private slots:
        void emulatedScreenUpdate();
        void statusBarUpdate();

        void on_actionExit_triggered();
        void on_actionInput_triggered();
        void on_actionOpen_triggered();
        void on_actionPause_triggered();
        void on_actionRun_triggered();
};

#endif // MAINWINDOW_H

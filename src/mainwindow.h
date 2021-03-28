#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>
#include <QSettings>
#include <QMainWindow>

#include "platform.h"
#include "opengl/opengl.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    public slots:

    private:
        Ui::MainWindow *ui;
        OpenGlWidget *openGlWidget;

        QTimer *screenUpdateTimer;
        QTimer *statusBarTimer;
        Platform *platform;

    private slots:
        void on_actionOpen_triggered();
        void on_actionExit_triggered();
        void on_actionRun_triggered();
        void on_actionPause_triggered();

        void emulatedScreenUpdate();
        void statusBarUpdate();     
        void on_actionInput_triggered();
};

#endif // MAINWINDOW_H

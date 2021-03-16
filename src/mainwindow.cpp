#include "mainwindow.h"
#include "opengl/opengl.h"

#include "ui_mainwindow.h"

#include "platformsmap.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    OpenGlWidget *openGlWidget;

    ui->setupUi(this);

    openGlWidget = new OpenGlWidget(this); //(QOpenGLWidget *) this->ui->centralwidget;
    setCentralWidget(openGlWidget);

    platform = nullptr;
    statusBarTimer = new QTimer(this);
}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_actionOpen_triggered() {
    QString romFileFilter;
    QString romFilename;

    QSettings settings("Miracle Game Boy", "None");

    romFileFilter = "Game Boy ROMs (*.gb *.sgb *.gbc);; All Files (*.*)";
    romFilename = QFileDialog::getOpenFileName(nullptr, "Open ROM", settings.value("lastOpenFilePath").toString(), romFileFilter);

    QFile romHandle(romFilename);

    romHandle.open(QIODevice::ReadOnly);

    if (romFilename != "" && !romHandle.isReadable()) {
        QMessageBox message;
        message.setText("Could not open file: " + romFilename);
        message.exec();
    }
    else if (romHandle.isReadable()) {
        settings.setValue("lastOpenFilePath", QFileInfo(romFilename).path());

        if (platform != nullptr) {
            delete platform;
        }

        platform = new Platform(availablePlatforms[ui->SystemType->checkedAction()->text().toStdString()]);
        platform->loadRomFile(romHandle.readAll());

        delete statusBarTimer;
        statusBarTimer = new QTimer(this);
    }
}


void MainWindow::on_actionExit_triggered() {
    QApplication::quit();
}


void MainWindow::on_actionRun_triggered()
{
    if (platform != nullptr) {
        QTimer *statusBarTimer = new QTimer(this);
        connect(statusBarTimer, SIGNAL(timeout()), this, SLOT(statusBarUpdate()));
        statusBarTimer->start(1000);
        platform->start();
    }
}


void MainWindow::on_actionPause_triggered()
{
    if (platform != nullptr)
        platform->stop();
}


void MainWindow::statusBarUpdate() {
    if (platform->getErrorMessage() == "") {
        ui->statusbar->showMessage("FPS: " + QString::number(platform->getFPS()));
    }
    else {
        ui->statusbar->showMessage("The platform encountered an error of: " + platform->getErrorMessage());
    }
    platform->resetFPS();
}

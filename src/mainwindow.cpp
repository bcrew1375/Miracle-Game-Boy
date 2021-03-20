#include "mainwindow.h"

#include "ui_mainwindow.h"

#include "platformsmap.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);

    openGlWidget = new OpenGlWidget(this); //(QOpenGLWidget *) this->ui->centralwidget;
    setCentralWidget(openGlWidget);

    platform = nullptr;
    statusBarTimer = nullptr;
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

        if (statusBarTimer != nullptr) {
            delete statusBarTimer;
        }

        platform = new Platform(availablePlatforms[ui->SystemType->checkedAction()->text().toStdString()]);

        platform->loadRomFile(romHandle.readAll());

        statusBarTimer = new QTimer(this);

        connect(platform, SIGNAL(screenUpdate()), this, SLOT(emulatedScreenUpdate()));
        connect(statusBarTimer, SIGNAL(timeout()), this, SLOT(statusBarUpdate()));
    }
}


void MainWindow::on_actionExit_triggered() {
    QApplication::quit();
}


void MainWindow::on_actionRun_triggered()
{
    if (platform != nullptr) {
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


void MainWindow::emulatedScreenUpdate()
{
    openGlWidget->updateEmulatedScreen(platform->getFrameBuffer());
}

#include "mainwindow.h"
#include "opengl/opengl.h"

#include "ui_mainwindow.h"

#include "platform.h"
#include "platformsmap.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    OpenGlWidget *openGlWidget;

    ui->setupUi(this);

    openGlWidget = new OpenGlWidget(this); //(QOpenGLWidget *) this->ui->centralwidget;
    setCentralWidget(openGlWidget);
}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_actionOpen_triggered() {
    Platform *platform;
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
    else {
        settings.setValue("lastOpenFilePath", QFileInfo(romFilename).path());
    }

    platform = new Platform(availablePlatforms[ui->SystemType->checkedAction()->text().toStdString()]);

    platform->loadRomFile(romHandle.readAll());
}


void MainWindow::on_actionExit_triggered() {
    QApplication::quit();
}


void MainWindow::on_actionRun_triggered()
{
}

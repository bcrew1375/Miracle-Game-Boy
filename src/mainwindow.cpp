#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "platformsmap.h"

#include <memory>

#include <QBuffer>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openGlWidget = new OpenGlWidget(this);
    setCentralWidget(openGlWidget);

    platform = nullptr;
    statusBarTimer = nullptr;
}


MainWindow::~MainWindow()
{
}


void MainWindow::statusBarUpdate()
{
    if (platform->getErrorMessage() == "")
    {
        ui->statusbar->showMessage("FPS: " + QString::number(platform->getFPS()));
    }
    else
    {
        ui->statusbar->showMessage("The platform encountered an error of: " + platform->getErrorMessage());
    }
    platform->resetFPS();
}


void MainWindow::emulatedScreenUpdate()
{
    openGlWidget->updateEmulatedScreen(platform->getFrameBuffer().get());
}


void MainWindow::on_actionExit_triggered()
{
    if (platform != nullptr)
        platform->pause();

    QApplication::quit();
}


void MainWindow::on_actionInput_triggered()
{

}


void MainWindow::on_actionOpen_triggered()
{
    QString romFileFilter;
    QString romFilename;

    QSettings settings("Miracle Game Boy", "None");

    romFileFilter = "Game Boy ROMs (*.gb *.sgb *.gbc);; All Files (*.*)";
    romFilename = QFileDialog::getOpenFileName(nullptr, "Open ROM", settings.value("lastOpenFilePath").toString(), romFileFilter);

    QFile romHandle(romFilename);
    QFile bootROM("E:/ROMS/Game Boy and Game Boy Color/test roms/dmg_boot.bin");

    romHandle.open(QIODevice::ReadOnly);
    bootROM.open(QIODevice::ReadOnly);

    if (romFilename != "" && !romHandle.isReadable())
    {
        QMessageBox message;
        message.setText("Could not open file: " + romFilename);
        message.exec();
    }
    else if (romHandle.isReadable())
    {
        settings.setValue("lastOpenFilePath", QFileInfo(romFilename).path());

        if (platform != nullptr)
        {
            platform->pause();
            ui->actionRun->setDisabled(false);
            openGlWidget->clearEmulatedScreen();
            //delete platform;
        }

        //if (statusBarTimer != nullptr)
            //delete statusBarTimer;

        platform = std::unique_ptr<Platform>(new Platform(availablePlatforms[ui->SystemType->checkedAction()->text().toStdString()]));
        statusBarTimer = std::unique_ptr<QTimer>(new QTimer(this));

        platform->loadRomFile(romFilename, bootROM.readAll(), romHandle.readAll());

        connect(platform.get(), SIGNAL(screenUpdate()), this, SLOT(emulatedScreenUpdate()));
        connect(statusBarTimer.get(), SIGNAL(timeout()), this, SLOT(statusBarUpdate()));

        ui->actionRun->setDisabled(true);
        statusBarTimer->start(1000);
        installEventFilter(platform.get());
        platform->start();
    }
}


void MainWindow::on_actionPause_triggered()
{
    ui->actionRun->setDisabled(false);

    if (platform != nullptr)
        platform->pause();
}


void MainWindow::on_actionRun_triggered()
{
    if (platform != nullptr)
    {
        ui->actionRun->setDisabled(true);
        statusBarTimer->start(1000);
        installEventFilter(platform.get());
        platform->start();
    }
}

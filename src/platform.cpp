#include <iostream>
#include <memory>

#include <QApplication>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QKeyEvent>
#include <QTimer>
#include <QMessageBox>

#include "platform.h"


Platform::Platform(int systemType) {
    saveDirectory = "/saves/";
    speedRegulationTimer = new QElapsedTimer();

    resetFPS();

    frameLocked = false;
    isRunning = false;

    errorMessage = "";
}


Platform::~Platform()
{
}


uint16_t Platform::getFPS() const
{
    return FPS;
}


std::shared_ptr<uint32_t[]> Platform::getFrameBuffer() const
{
    return system->getFrameBuffer();
}


void Platform::loadRomFile(QString romFilename, QByteArray bootROM, QByteArray romData) {
    QByteArray saveData;
    saveFilename = saveDirectory;

    // Remove the directory and extension from the filename.
    saveFilename.append(romFilename.right(romFilename.length() - (romFilename.lastIndexOf("/") + 1)));
    saveFilename.chop(saveFilename.length() - saveFilename.lastIndexOf("."));

    saveFilename.append(".sav");

    saveData = readSaveRamFromFile();

    try
    {
        std::unique_ptr bootROMArray = std::make_unique<uint8_t[]>(bootROM.size());
        std::shared_ptr romDataArray = std::make_shared<uint8_t[]>(romData.size());
        std::shared_ptr saveDataArray = std::make_shared<uint8_t[]>(saveData.size());

        std::copy(bootROM.data(), bootROM.data() + bootROM.size(), bootROMArray.get());
        std::copy(romData.data(), romData.data() + romData.size(), romDataArray.get());
        std::copy(saveData.data(), saveData.data() + saveData.size(), saveDataArray.get());

        system = std::make_unique<System>(
            std::move(bootROMArray),
            romDataArray,
            romData.size(),
            saveDataArray,
            saveData.length());
    }
    catch (std::exception e)
    {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void Platform::pause() {
    isRunning = false;
}


QByteArray Platform::readSaveRamFromFile() const
{
    QDir *directory = new QDir();
    QFile *saveFile = new QFile(directory->currentPath().append(saveFilename));
    QByteArray saveData;

    if (saveFile->exists())
    {
        saveFile->open(QIODevice::ReadOnly);
        saveData = saveFile->readAll();
        saveFile->close();
    }

    return saveData;
}


void Platform::resetFPS() {
    FPS = 0;
}


void Platform::setSystemType() {

}


void Platform::start() {
    nanoSecondsPerFrame = 1000000000 / system->getRefreshRate();
    milliSecondsPerFrame = (double)nanoSecondsPerFrame / 1000000;
    timeElapsed = 0;

    isRunning = true;

    speedRegulationTimer->start();

    executionLoop();
}


void Platform::writeSaveRamToFile()
{
    QDir *directory = new QDir();
    QFile *saveFile = new QFile(directory->currentPath().append(saveFilename));
    QByteArray saveData;
    qint32 saveDataSize;

    saveDataSize = system->getSaveDataSize();
    saveData = saveData.fromRawData(reinterpret_cast<char *>(system->getSaveData().get()), saveDataSize);

    if (!directory->exists(directory->currentPath() + saveDirectory))
        directory->mkdir(directory->currentPath() + saveDirectory);

    if (saveData != nullptr)
    {
        saveFile->open(QIODevice::WriteOnly);
        saveFile->write(saveData, saveDataSize);
        saveFile->close();
    }
}


void Platform::executionLoop() {
    qint8 timeDelay;

    if (frameLocked == true)
        timeDelay = round(milliSecondsPerFrame - (speedRegulationTimer->nsecsElapsed() / nanoSecondsPerFrame));
    else
        timeDelay = 1;

    if (timeDelay <= 0)
        timeDelay = 1;

    timeElapsed += speedRegulationTimer->restart();

    // This should be moved to the end of the function to prevent possible premature recursion.
    if (isRunning == true)
        QTimer::singleShot(timeDelay, Qt::PreciseTimer, this, SLOT(executionLoop()));

    system->setControllerInputs(buttonInputs);
    system->executeCycles();
    if (system->getIsRunning() == false) {
        this->pause();
        errorMessage = QString::fromStdString(system->getSystemError());
    }
    // Ensure the screen only updates at the system refresh rate
    // even if the emulation is running faster.
    if (timeElapsed >= milliSecondsPerFrame)
    {
        emit screenUpdate();
        timeElapsed -= milliSecondsPerFrame;
    }
    writeSaveRamToFile();
    FPS++;
}


bool Platform::eventFilter(QObject *obj, QEvent *event)
{
    QKeyEvent *keyEvent;

    if (event->type() == QEvent::KeyPress)
    {
        keyEvent = (QKeyEvent *)event;

        switch (keyEvent->key())
        {
        case Qt::Key_Down: keyEvent->accept(); buttonInputs[0] = true; break;
        case Qt::Key_Up: keyEvent->accept(); buttonInputs[1] = true; break;
        case Qt::Key_Left: keyEvent->accept(); buttonInputs[2] = true; break;
        case Qt::Key_Right: keyEvent->accept(); buttonInputs[3] = true; break;
        case Qt::Key_Return: keyEvent->accept(); buttonInputs[4] = true; break;
        case Qt::Key_Shift: keyEvent->accept(); buttonInputs[5] = true; break;
        case Qt::Key_Z: keyEvent->accept(); buttonInputs[6] = true; break;
        case Qt::Key_X: keyEvent->accept(); buttonInputs[7] = true; break;

        case Qt::Key_Tab: frameLocked ^= true; break;
        default: return false; break;
        }
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        keyEvent = (QKeyEvent *)event;

        switch (keyEvent->key())
        {
        case Qt::Key_Down: keyEvent->accept(); buttonInputs[0] = false; break;
        case Qt::Key_Up: keyEvent->accept(); buttonInputs[1] = false; break;
        case Qt::Key_Left: keyEvent->accept(); buttonInputs[2] = false; break;
        case Qt::Key_Right: keyEvent->accept(); buttonInputs[3] = false; break;
        case Qt::Key_Return: keyEvent->accept(); buttonInputs[4] = false; break;
        case Qt::Key_Shift: keyEvent->accept(); buttonInputs[5] = false; break;
        case Qt::Key_Z: keyEvent->accept(); buttonInputs[6] = false; break;
        case Qt::Key_X: keyEvent->accept(); buttonInputs[7] = false; break;
        default: return false; break;
        }
    }

    return false;
}


QString Platform::getErrorMessage() const {
    return errorMessage;
}

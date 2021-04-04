#include <iostream>

#include <QApplication>

#include "platform.h"


Platform::Platform(int systemType) {
    speedRegulationTimer = new QTimer(this);
    connect(speedRegulationTimer, SIGNAL(timeout()), this, SLOT(executionLoop()));

    resetFPS();

    errorMessage = "";
}


Platform::~Platform()
{
    delete speedRegulationTimer;
    delete system;
}


void Platform::loadRomFile(QByteArray bootROM, QByteArray romData) {
    system = new System((uint8_t *)bootROM.constData(), (uint8_t *)romData.constData(), romData.size());
}


void Platform::pause() {

}


void Platform::setSystemType() {

}


void Platform::start() {
    //speedRegulationTimer->start(round(1000 / system->getRefreshRate())); // Tie the emulation speed to the number of screen refreshes per second.

    while (0 == 0) {
        QApplication::processEvents();
        //speedRegulationTimer->start(round(1000 / system->getRefreshRate())); // Tie the emulation speed to the number of screen refreshes per second.
        executionLoop();
    }
}


void Platform::executionLoop() {
    // Execute the cycles of the emulated system for one frame.
    system->setControllerInputs(buttonInputs);
    system->executeCycles();
    if (system->getIsRunning() == false) {
        this->stop();
        errorMessage = QString::fromStdString(system->getSystemError());
    }
    FPS++;
}


void Platform::stop() {
    speedRegulationTimer->stop();
}


uint16_t Platform::getFPS()
{
    return FPS;
}


uint32_t *Platform::getFrameBuffer()
{
    return system->getFrameBuffer();
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
        default: return false; break;
        }

        //system->setControllerInputs(buttonInputs);

        //memset(buttonInputs, false, 8);
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

        //system->setControllerInputs(buttonInputs);
    }

    return false;
}


void Platform::resetFPS() {
    FPS = 0;
}


QString Platform::getErrorMessage() {
    return errorMessage;
}

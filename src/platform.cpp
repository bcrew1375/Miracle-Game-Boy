#include <iostream>

#include <QApplication>

#include "platform.h"


Platform::Platform(int systemType) {
    speedRegulationTimer = new QTimer(this);
    connect(speedRegulationTimer, SIGNAL(timeout()), this, SLOT(executionLoop()));
//    connect(speedRegulationTimer, SIGNAL(timeout()), this->parent(), SLOT(emulatedScreenUpdate()));

    resetFPS();

    errorMessage = "";
}


void Platform::loadRomFile(QByteArray romData) {
    system = new System((uint8_t *)romData.constData(), romData.size());
}


void Platform::pause() {

}


void Platform::setSystemType() {

}


void Platform::start() {
    speedRegulationTimer->start(round(1000 / system->getRefreshRate())); // Tie the emulation speed to the number of screen refreshes per second.

    /*while (0 == 0) {
        QApplication::processEvents();
        //speedRegulationTimer->start(round(1000 / system->getRefreshRate())); // Tie the emulation speed to the number of screen refreshes per second.
        executionLoop();
    }*/
}


void Platform::executionLoop() {
    // Execute the cycles of the emulated system for one frame.
    system->executeCycles();
    if (system->getIsRunning() == false) {
        this->stop();
        errorMessage = QString::fromStdString(system->getSystemError());
    }
    FPS++;
    emit screenUpdate();
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


void Platform::resetFPS() {
    FPS = 0;
}


QString Platform::getErrorMessage() {
    return errorMessage;
}

#include "platform.h"
#include <iostream>

Platform::Platform(int systemType) {
}


void Platform::loadRomFile(QByteArray romData) {
    system = new System((uint8_t *)romData.constData(), romData.size());
}


void Platform::pause() {

}


void Platform::setSystemType() {

}


void Platform::start() {
    isRunning = true;
}


void Platform::stop() {

}

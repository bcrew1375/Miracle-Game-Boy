#ifndef PLATFORM_H
#define PLATFORM_H

#include <QByteArray>

#include <stdint.h>

#include "gbemu/system.h"

class Platform {
    public:
        Platform(int systemType);

        bool isRunning;
        bool isPaused;

        void loadRomFile(QByteArray romData);
        void pause();
        void setSystemType();
        void start();
        void stop();

    private:
        System *system;
        uint8_t *romBuffer;
        uint8_t romSizeInBytes;
};

#endif // PLATFORM_H

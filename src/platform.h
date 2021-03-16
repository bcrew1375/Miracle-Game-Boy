#ifndef PLATFORM_H
#define PLATFORM_H

#include <QByteArray>
#include <QTimer>

#include <stdint.h>

#include "gbemu/system.h"

class Platform : public QObject {
    Q_OBJECT

    public:
        Platform(int systemType);

        bool isRunning;
        bool isPaused;

        void loadRomFile(QByteArray romData);
        void pause();
        void setSystemType();
        void start();
        void stop();
        void resetFPS();
        uint16_t getFPS();

    private:
        System *system;
        QTimer *speedRegulationTimer;
        uint8_t *romBuffer;
        uint8_t romSizeInBytes;
        uint16_t FPS;

    private slots:
        void executionLoop();
};

#endif // PLATFORM_H

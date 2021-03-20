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
        QString getErrorMessage();
        uint16_t getFPS();
        uint32_t *getFrameBuffer();

    private:
        System *system;
        QTimer *speedRegulationTimer;
        uint8_t *romBuffer;
        uint8_t romSizeInBytes;
        uint16_t FPS;

        QString errorMessage;

    private slots:
        void executionLoop();

    signals:
        void screenUpdate();
};

#endif // PLATFORM_H

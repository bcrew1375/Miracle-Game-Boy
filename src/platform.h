#ifndef PLATFORM_H
#define PLATFORM_H

#include <QWidget>
#include <QByteArray>
#include <QTimer>
#include <QElapsedTimer>
#include <QKeyEvent>

#include <stdint.h>

#include "gbemu/system.h"

class Platform : public QWidget {
    Q_OBJECT

    public:
        Platform(int systemType);
        ~Platform();

        QString getErrorMessage();

        bool isPaused;
        bool isRunning;

        uint16_t getFPS();
        uint32_t *getFrameBuffer();

        void loadRomFile(QByteArray bootROM, QByteArray romData);
        void pause();
        void resetFPS();
        void setSystemType();
        void start();
        void stop();

    protected:
        virtual bool eventFilter(QObject *obj, QEvent *event);

    private:
        QElapsedTimer *speedRegulationTimer;
        QString errorMessage;

        System *system;

        bool frameLocked;
        bool buttonInputs[8] = { false, false, false, false, false, false, false, false };

        double  milliSecondsPerFrame;

        quint64 nanoSecondsPerFrame;
        quint64 timeElapsed;

        uint16_t FPS;

        uint8_t *romBuffer;
        uint8_t romSizeInBytes;

    private slots:
        void executionLoop();

    signals:
        void screenUpdate();
};

#endif // PLATFORM_H

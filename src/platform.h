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

        bool isRunning;
        bool isPaused;

        void loadRomFile(QByteArray bootROM, QByteArray romData);
        void pause();
        void setSystemType();
        void start();
        void stop();
        void resetFPS();
        QString getErrorMessage();
        uint16_t getFPS();
        uint32_t *getFrameBuffer();

    protected:
        virtual bool eventFilter(QObject *obj, QEvent *event);

    private:
        System *system;
        //QTimer *speedRegulationTimer;
        QElapsedTimer *speedRegulationTimer;
        uint8_t *romBuffer;
        uint8_t romSizeInBytes;
        uint16_t FPS;
        bool platformRunning;

        QString errorMessage;

        bool buttonInputs[8] = { false, false, false, false, false, false, false, false };

    private slots:
        void executionLoop();

    signals:
        void screenUpdate();
};

#endif // PLATFORM_H

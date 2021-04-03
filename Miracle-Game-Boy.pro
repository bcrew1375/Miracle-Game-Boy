QT += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/gbemu/cpu.cpp \
    src/gbemu/display.cpp \
    src/gbemu/instructionset.cpp \
    src/gbemu/instructionsetcb.cpp \
    src/gbemu/ioports.cpp \
    src/gbemu/memory.cpp \
    src/gbemu/memorybankcontroller.cpp \
    src/gbemu/system.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/opengl/opengl.cpp \
    src/platform.cpp

HEADERS += \
    src/gbemu/cpu.h \
    src/gbemu/cycletables.h \
    src/gbemu/display.h \
    src/gbemu/ioports.h \
    src/gbemu/memory.h \
    src/gbemu/memorybankcontroller.h \
    src/gbemu/system.h \
    src/mainwindow.h \
    src/opengl/opengl.h \
    src/platform.h \
    src/platformsmap.h \

FORMS += \
    src/mainwindow.ui \

TRANSLATIONS += \
    Miracle-Gameboy-OOP_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    src/Miracle-Gameboy-OOP_en_US.ts \
    src/opengl/shaders/default_fragment.glsl \
    src/opengl/shaders/default_vertex.glsl

INCLUDEPATH += $$PWD/src/GLFW
DEPENDPATH += $$PWD/src/GLFW


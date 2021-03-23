#include <QApplication>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication miracleGameBoy(argc, argv);
    MainWindow window;
    window.show();
    qApp->setQuitOnLastWindowClosed(true);
    return miracleGameBoy.exec();
}

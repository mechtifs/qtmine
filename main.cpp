#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    int currentExitCode = 0;
    QApplication a(argc, argv);
    MainWindow w;
    currentExitCode = a.exec();
    return currentExitCode;
}

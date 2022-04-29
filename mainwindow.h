#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QElapsedTimer>
#include <QMainWindow>
#include "enddiaalog.h"
#include "minefield.h"
#include "startdialog.h"


class MainWindow : public QMainWindow {
    Q_OBJECT
    StartDialog *s;
    MineField *minefield;
    EndDialog *e;
public:
    static int w;
    static int h;
    static int n;
    MainWindow(QWidget *parent = nullptr);
public slots:
    void start();
    void show();
    void showEnd(bool hasWon = false);
    void quit();
    void restart();
};
#endif // MAINWINDOW_H

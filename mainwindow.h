#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "enddialog.h"
#include "menudialog.h"
#include "minefield.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
    MenuDialog *m;
    MineField *minefield = NULL;
    EndDialog *e;
    int initialTime = 0;
    bool isEnded = false;
public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void enable();
    void start();
    void show();
    void show(int mines, int left, int time, QBitArray *mineData);
    void showEnd(bool hasWon = false);
    void quit();
    void showMenu();
};

#endif // MAINWINDOW_H

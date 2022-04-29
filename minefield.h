#ifndef MINEFIELD_H
#define MINEFIELD_H

#include <QScrollArea>
#include <QRandomGenerator>
#include <QElapsedTimer>
#include <QThread>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLCDNumber>
#include <QPushButton>
#include <QTimer>
#include <QTime>
#include <QApplication>
#include "minebutton.h"


class MineField: public QWidget {
    Q_OBJECT
    int w;
    int h;
    int n;
    int left;
    int flags = 0;
    QWidget * Widget;
    QHBoxLayout *HBox;
    QVBoxLayout *VBox;
    QFrame *Frame;
    QScrollArea *Scroll;
    QLCDNumber *Mines;
    QLCDNumber *Time;
    QGridLayout *Grid;
    QPushButton *Restart;
    MineButton ***mine;
public:
    MineField(int w, int h, int n);
    QElapsedTimer *elapsedTimer;
    QTimer *timer;
    void setSize(int w, int h, int n);
public slots:
    int countMines(int x, int y);
    void recursiveFlatten(int x, int y);
    void updateFlags(int x, int y, int flag);
    void setMines(int x, int y);
    void showMines(bool hasWon = false);
    void onTimerOut();
signals:
    void requestEnd(bool hasWon = false);
    void requestRestart();
};

#endif // MINEFIELD_H

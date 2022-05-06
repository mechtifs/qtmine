#ifndef MINEFIELD_H
#define MINEFIELD_H

#include <QScrollArea>
#include <QRandomGenerator>
#include <QThread>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLCDNumber>
#include <QPushButton>
#include <QTimer>
#include <QApplication>
#include <QBitArray>
#include "minebutton.h"


class MineField: public QWidget {
    Q_OBJECT
    int w;
    int h;
    int n;
    int flags = 0;
    QWidget * widget;
    QHBoxLayout *hBox;
    QVBoxLayout *vBox;
    QFrame *frame;
    QScrollArea *scroll;
    QGridLayout *grid;
    QPushButton *menuButton;
    MineButton ***mine;
public:
    MineField(int w, int h, int n, int left = 0, int initialTime = 0, QBitArray * saveData = NULL);
    QTimer *timer;
    QLCDNumber *mineLCD;
    QLCDNumber *timeLCD;
    QBitArray *exportData();
    void setSize(int w, int h, int n);
private slots:
    int countMines(int x, int y);
    void recursiveFlatten(int x, int y);
    void updateFlags(int x, int y, int flag);
    void setMines(int x, int y);
    void showMines(bool hasWon = false);
    void onTimerOut();
signals:
    void requestEnd(bool hasWon = false);
    void requestMenu();
};

#endif // MINEFIELD_H

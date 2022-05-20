#ifndef MINEBUTTON_H
#define MINEBUTTON_H

#include <QPushButton>
#include <QMouseEvent>


class MineButton: public QPushButton {
    Q_OBJECT
    void mouseReleaseEvent(QMouseEvent *e);
public:
    MineButton(int i, int j);
    static bool isFirst;
    static int left;
    int x;
    int y;
    bool hasMine = false;
    void flatten(int num = 0);
    void flag();
    bool isFlagged();
signals:
    void requestSetMines(int x, int y);
    void requestFlatten(int x, int y);
    void requestFlag(int x, int y, int flag);
    void requestEnd(bool hasWon = false);
};

#endif // MINEBUTTON_H

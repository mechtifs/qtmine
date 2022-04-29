#include "minefield.h"

int MineButton::left;
bool MineButton::isFirst;

MineField::MineField(int w, int h, int n): w(w), h(h), n(n) {
    int i, j;
//    setMaximumSize(25 * w + 22, 25 * h + 78);
    Frame = new QFrame;
    HBox = new QHBoxLayout(Frame);
    VBox = new QVBoxLayout;
    Scroll = new QScrollArea;
//    VBox->setSizeConstraint(QLayout::SetMinAndMaxSize);
    Widget = new QWidget;
    Grid = new QGridLayout(Widget);
    Time = new QLCDNumber;
    Mines = new QLCDNumber;
    Restart = new QPushButton(":)");
    Widget->setFixedSize(25 * w + 15, 25 * h + 15);
    Frame->setFrameShape(QFrame::Panel);
    Frame->setFrameShadow(QFrame::Sunken);
    Scroll->setFrameShape(QFrame::Panel);
    Scroll->setFrameShadow(QFrame::Sunken);
    Scroll->setLineWidth(2);
    Scroll->setAlignment(Qt::AlignCenter);
    Scroll->setWidget(Widget);
    HBox->addWidget(Mines);
    HBox->addStretch();
    HBox->addWidget(Restart);
    HBox->addStretch();
    HBox->addWidget(Time);
    VBox->addWidget(Frame);
    VBox->addWidget(Scroll);
    Grid->setSpacing(1);
    Time->setSegmentStyle(QLCDNumber::Flat);
    Mines->setSegmentStyle(QLCDNumber::Flat);
    Mines->display(n);
    connect(Restart, SIGNAL(clicked()), this, SIGNAL(requestRestart()));
    MineButton::left = w * h - n;
    MineButton::isFirst = true;
    mine = new MineButton **[w];
    for (i = 0; i < w; i++) {
        mine[i] = new MineButton *[h];
        for (j = 0; j < h; j++) {
            mine[i][j] = new MineButton(i, j);
            Grid->addWidget(mine[i][j], j, i);
            connect(mine[i][j], SIGNAL(requestFlatten(int, int)), this, SLOT(recursiveFlatten(int, int)));
            connect(mine[i][j], SIGNAL(requestFlag(int, int, int)), this, SLOT(updateFlags(int, int, int)));
            connect(mine[i][j], SIGNAL(requestSetMines(int, int)), this, SLOT(setMines(int, int)));
            connect(mine[i][j], SIGNAL(requestEnd(bool)), this, SIGNAL(requestEnd(bool)));
            connect(mine[i][j], SIGNAL(requestEnd(bool)), this, SLOT(showMines(bool)));
        }
    }
    setLayout(VBox);
}

void MineField::setMines(int x, int y) {
    int u, v, i = 0;
    while (i < n) {
        u = QRandomGenerator::global()->bounded(w);
        v = QRandomGenerator::global()->bounded(h);
        if ((x != u || y != v) && !mine[u][v]->hasMine) {
            mine[u][v]->hasMine = true;
//            mine[u][v]->setText("X");
            i++;
        }
    }
    timer = new QTimer;
    timer->setInterval(1000);
    elapsedTimer = new QElapsedTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
    timer->start();
    elapsedTimer->start();
    recursiveFlatten(x, y);
}

void MineField::recursiveFlatten(int x, int y) {
    int cnt = countMines(x, y);
    if (!mine[x][y]->isFlat() && mine[x][y]->text() == "") {
        MineButton::left--;
        if (cnt) {
            mine[x][y]->flatten(cnt);
        } else {
            mine[x][y]->flatten();
            if (x > 0 && y > 0 && !mine[x-1][y-1]->hasMine) {
                recursiveFlatten(x - 1, y - 1);
            }
            if (x > 0 && !mine[x-1][y]->hasMine) {
                recursiveFlatten(x - 1, y);
            }
            if (x > 0 && y < h - 1 && !mine[x-1][y+1]->hasMine) {
                recursiveFlatten(x - 1, y + 1);
            }
            if (y > 0 && !mine[x][y-1]->hasMine) {
                recursiveFlatten(x, y - 1);
            }
            if (y < h - 1 && !mine[x][y+1]->hasMine) {
                recursiveFlatten(x, y + 1);
            }
            if (x < w - 1 && y > 0 && !mine[x+1][y-1]->hasMine) {
                recursiveFlatten(x + 1, y - 1);
            }
            if (x < w - 1 && !mine[x+1][y]->hasMine) {
                recursiveFlatten(x + 1, y);
            }
            if (x < w - 1 && y < h - 1 && !mine[x+1][y+1]->hasMine) {
                recursiveFlatten(x + 1, y + 1);
            }
        }
    }
}

int MineField::countMines(int x, int y) {
    int cnt = 0;
    if (x > 0 && y > 0 && mine[x-1][y-1]->hasMine) {
        cnt++;
    }
    if (x > 0 && mine[x-1][y]->hasMine) {
        cnt++;
    }
    if (x > 0 && y < h - 1 && mine[x-1][y+1]->hasMine) {
        cnt++;
    }
    if (y > 0 && mine[x][y-1]->hasMine) {
        cnt++;
    }
    if (y < h - 1 && mine[x][y+1]->hasMine) {
        cnt++;
    }
    if (x < w - 1 && y > 0 && mine[x+1][y-1]->hasMine) {
        cnt++;
    }
    if (x < w - 1 && mine[x+1][y]->hasMine) {
        cnt++;
    }
    if (x < w - 1 && y < h - 1 && mine[x+1][y+1]->hasMine) {
        cnt++;
    }
    return cnt;
}

void MineField::updateFlags(int x, int y, int flag) {
    Mines->display(Mines->intValue() - flag);
    if (Mines->intValue() == -1) {
        QPalette lcdpat = Mines->palette();
        lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::red);
        Mines->setPalette(lcdpat);
    }
    if (Mines->intValue() == 0) {
        QPalette lcdpat;
        Mines->setPalette(lcdpat);
    }
    if (mine[x][y]->hasMine) {
        flags += flag;
        if (flags == n) {
            timer->stop();
            emit requestEnd(true);
        }
    }
}

void MineField::showMines(bool hasWon) {
    int i, j;
    timer->stop();
    QString res = "X";
    if (hasWon) {
        Mines->display(0);
        res = "*";
    } else {
        Restart->setText(":(");
    }
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            if (mine[i][j]->hasMine && mine[i][j]->text() == "") {
                mine[i][j]->setText(res);
            }
        }
    }
}

void MineField::onTimerOut() {
    Time->display(Time->intValue() + 1);
}

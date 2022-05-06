#include "minefield.h"

int MineButton::left;
bool MineButton::isFirst;

MineField::MineField(int w, int h, int n, int l, int initialTime, QBitArray * saveData): w(w), h(h), n(n) {
    int i, j;
    frame = new QFrame;
    hBox = new QHBoxLayout(frame);
    vBox = new QVBoxLayout;
    scroll = new QScrollArea;
    widget = new QWidget;
    grid = new QGridLayout(widget);
    timeLCD = new QLCDNumber;
    mineLCD = new QLCDNumber;
    menuButton = new QPushButton(":)");
    timer = new QTimer;
    widget->setFixedSize(25 * w + 15, 25 * h + 15);
    frame->setFrameShape(QFrame::Panel);
    frame->setFrameShadow(QFrame::Sunken);
    scroll->setFrameShape(QFrame::Panel);
    scroll->setFrameShadow(QFrame::Sunken);
    scroll->setLineWidth(2);
    scroll->setAlignment(Qt::AlignCenter);
    scroll->setWidget(widget);
    hBox->addWidget(mineLCD);
    hBox->addStretch();
    hBox->addWidget(menuButton);
    hBox->addStretch();
    hBox->addWidget(timeLCD);
    vBox->addWidget(frame);
    vBox->addWidget(scroll);
    grid->setSpacing(1);
    timeLCD->setSegmentStyle(QLCDNumber::Flat);
    timeLCD->display(initialTime / 1000);
    mineLCD->setSegmentStyle(QLCDNumber::Flat);
    mineLCD->display(n);
    connect(menuButton, SIGNAL(clicked()), this, SIGNAL(requestMenu()));
    MineButton::left = w * h - n;
    mine = new MineButton **[w];
    for (i = 0; i < w; i++) {
        mine[i] = new MineButton *[h];
        for (j = 0; j < h; j++) {
            mine[i][j] = new MineButton(i, j);
            grid->addWidget(mine[i][j], j, i);
            connect(mine[i][j], SIGNAL(requestFlatten(int, int)), this, SLOT(recursiveFlatten(int, int)));
            connect(mine[i][j], SIGNAL(requestFlag(int, int, int)), this, SLOT(updateFlags(int, int, int)));
            connect(mine[i][j], SIGNAL(requestSetMines(int, int)), this, SLOT(setMines(int, int)));
            connect(mine[i][j], SIGNAL(requestEnd(bool)), this, SIGNAL(requestEnd(bool)));
            connect(mine[i][j], SIGNAL(requestEnd(bool)), this, SLOT(showMines(bool)));
        }
    }
    MineButton::isFirst = !saveData;
    if (saveData) {
        for (i = 0; i < w; i++) {
            for (j = 0; j < h; j++) {
                if ((*saveData)[(i * h + j) * 2]) {
                    mine[i][j]->hasMine = true;
                }
            }
        }
        for (i = 0; i < w; i++) {
            for (j = 0; j < h; j++) {
                if ((*saveData)[(i * h + j) * 2 + 1]) {
                    if (mine[i][j]->hasMine) {
                        mine[i][j]->setText("*");
                    } else {
                        mine[i][j]->flatten(countMines(i, j));
                    }
                }
            }
        }
        MineButton::left = l;
        connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
        timer->start(1000);
    }
    setLayout(vBox);
}

void MineField::setMines(int x, int y) {
    int u, v, i = 0;
    while (i < n) {
        u = QRandomGenerator::global()->bounded(w);
        v = QRandomGenerator::global()->bounded(h);
        if ((x != u || y != v) && !mine[u][v]->hasMine) {
            mine[u][v]->hasMine = true;
            i++;
        }
    }
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
    timer->start(1000);
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
    mineLCD->display(mineLCD->intValue() - flag);
    if (mineLCD->intValue() == -1) {
        QPalette lcdpat = mineLCD->palette();
        lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::red);
        mineLCD->setPalette(lcdpat);
    }
    if (mineLCD->intValue() == 0) {
        QPalette lcdpat;
        mineLCD->setPalette(lcdpat);
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
        mineLCD->display(0);
        res = "*";
    } else {
        menuButton->setText(":(");
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
    timeLCD->display(timeLCD->intValue() + 1);
}

QBitArray *MineField::exportData() {
    QBitArray *mineData = new QBitArray(w * h * 2);
    int i, j;
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            mineData->setBit((i * h + j) * 2, mine[i][j]->hasMine);
            mineData->setBit((i * h + j) * 2 + 1, mine[i][j]->isFlat() || mine[i][j]->text() == "*");
        }
    }
    return mineData;
}

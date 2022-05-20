#include "minefield.h"

int MineButton::left;
bool MineButton::isFirst;

MineField::MineField(int w, int h, int n, int l, int initialTime, QBitArray *mineData): w(w), h(h), n(n) {
    // Initialize timer
    timer = new QTimer;
    // Initialize frame
    frame = new QFrame;
    frame->setFrameShape(QFrame::Panel);
    frame->setFrameShadow(QFrame::Sunken);
    // Initialize widget
    widget = new QWidget;
    widget->setFixedSize(25 * w + 15, 25 * h + 15);
    // Initialize scroll area
    scroll = new QScrollArea;
    scroll->setFrameShape(QFrame::Panel);
    scroll->setFrameShadow(QFrame::Sunken);
    scroll->setLineWidth(2);
    scroll->setAlignment(Qt::AlignCenter);
    scroll->setWidget(widget);
    // Initialize horizontal items
    mineLCD = new QLCDNumber;
    mineLCD->setSegmentStyle(QLCDNumber::Flat);
    mineLCD->display(n);
    menuButton = new QPushButton(":)");
    connect(menuButton, SIGNAL(clicked()), this, SIGNAL(requestMenu()));
    timeLCD = new QLCDNumber;
    timeLCD->setSegmentStyle(QLCDNumber::Flat);
    timeLCD->display(initialTime / 1000);
    // Initialize horizontal box layout
    hBox = new QHBoxLayout(frame);
    hBox->addWidget(mineLCD);
    hBox->addStretch();
    hBox->addWidget(menuButton);
    hBox->addStretch();
    hBox->addWidget(timeLCD);
    // Initialize vertical box layout
    vBox = new QVBoxLayout;
    vBox->addWidget(frame);
    vBox->addWidget(scroll);
    // Initialize grid layout
    grid = new QGridLayout(widget);
    grid->setSpacing(1);
    // Initialize mine buttons
    int i, j;
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
    MineButton::isFirst = !mineData;
    // Check if passing a data pointer
    if (mineData) {
        MineButton::left = l;
        loadData(mineData);
        connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
        timer->start(1000);
    } else {
        MineButton::left = w * h - n;
    }
    // Set main layout
    setLayout(vBox);
}

void MineField::loadData(QBitArray *mineData) {
    int i, j;
    // Set mines
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            if ((*mineData)[(i * h + j) * 3]) {
                mine[i][j]->hasMine = true;
            }
        }
    }
    // Set attributes
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            if ((*mineData)[(i * w + j) * 3 + 1]) {
                mine[i][j]->flatten(countMines(i, j));
            } else if ((*mineData)[(i * w + j) * 3 + 2]) {
                mine[i][j]->setText("*");
            }
        }
    }
}

void MineField::setMines(int x, int y) {
    // Generate random coordinates till mine number is fulfilled
    int u, v, i = 0;
    while (i < n) {
        u = QRandomGenerator::global()->bounded(w);
        v = QRandomGenerator::global()->bounded(h);
        if ((x != u || y != v) && !mine[u][v]->hasMine) {
            mine[u][v]->hasMine = true;
            i++;
        }
    }
    // Start timer
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
    timer->start(1000);
    recursiveFlatten(x, y);
}

void MineField::recursiveFlatten(int x, int y) {
    int cnt = countMines(x, y);
    // Check if the button could be flatten
    if (!mine[x][y]->isFlat() && !mine[x][y]->isFlagged()) {
        MineButton::left--;
        if (cnt) {
            mine[x][y]->flatten(cnt);
        } else {
            mine[x][y]->flatten();
            // Recursively flatten nearby buttons
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
    // Count nearby mines
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
    // Change display color to red when below zero
    if (mineLCD->intValue() == -1) {
        QPalette lcdpat = mineLCD->palette();
        lcdpat.setColor(QPalette::Normal,QPalette::WindowText,Qt::red);
        mineLCD->setPalette(lcdpat);
    } else if (mineLCD->intValue() == 0) {
        QPalette lcdpat;
        mineLCD->setPalette(lcdpat);
    }
    // True mine count
    if (mine[x][y]->hasMine) {
        flags += flag;
        // Win
        if (flags == n) {
            timer->stop();
            emit requestEnd(true);
        }
    }
}

void MineField::showMines(bool hasWon) {
    int i, j;
    timer->stop();
    // Display all mines
    QString res = "X";
    if (hasWon) {
        mineLCD->display(0);
        res = "*";
    } else {
        menuButton->setText(":(");
    }
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            if (mine[i][j]->hasMine && !mine[i][j]->isFlagged()) {
                mine[i][j]->setText(res);
            }
        }
    }
}

void MineField::onTimerOut() {
    timeLCD->display(timeLCD->intValue() + 1);
}

QBitArray *MineField::exportData() {
    int i, j;
    QBitArray *mineData = new QBitArray(w * h * 3);
    // Export the data of each button in three bits
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            mineData->setBit((i * w + j) * 3, mine[i][j]->hasMine);
            mineData->setBit((i * w + j) * 3 + 1, mine[i][j]->isFlat());
            mineData->setBit((i * w + j) * 3 + 2, mine[i][j]->text() == "*");
        }
    }
    return mineData;
}

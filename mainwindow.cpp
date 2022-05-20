#include "mainwindow.h"

int MenuDialog::w;
int MenuDialog::h;
int MenuDialog::n;
bool MenuDialog::disabled;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    // Set default value
    MenuDialog::w = 20;
    MenuDialog::h = 20;
    MenuDialog::n = 50;
    MenuDialog::disabled = true;
    start();
}

void MainWindow::start() {
    // Show menu dialog at start
    m = new MenuDialog;
    setWindowTitle("QtMine");
    connect(m, SIGNAL(requestShow()), this, SLOT(show()));
    connect(m, SIGNAL(requestShow(int, int, int, QBitArray *)), this, SLOT(show(int, int, int, QBitArray *)));
    m->exec();
    connect(minefield, SIGNAL(requestEnd(bool)), this, SLOT(showEnd(bool)));
}

void MainWindow::show() {
    // Show main window
    isEnded = false;
    minefield = new MineField(MenuDialog::w, MenuDialog::h, MenuDialog::n);
    setCentralWidget(minefield);
    resize(25 * MenuDialog::w + 37, 25 * MenuDialog::h + 93);
    connect(minefield, SIGNAL(requestMenu()), this, SLOT(showMenu()));
    QWidget::show();
}

void MainWindow::show(int mines, int left, int initialTime, QBitArray * mineData) {
    // Show main window with data
    if (minefield) {
        delete minefield;
    }
    isEnded = false;
    this->initialTime = initialTime;
    minefield = new MineField(MenuDialog::w, MenuDialog::h, mines, left, initialTime, mineData);
    setCentralWidget(minefield);
    resize(25 * MenuDialog::w + 37, 25 * MenuDialog::h + 93);
    connect(minefield, SIGNAL(requestMenu()), this, SLOT(showMenu()));
    QWidget::show();
}

void MainWindow::showEnd(bool hasWon) {
    // Show end dialog
    isEnded = true;
    setDisabled(true);
    e = new EndDialog;
    e->setModal(Qt::WindowModal);
    if (hasWon) {
        e->label->setText("Finished in " + QString::number(minefield->timeLCD->intValue() + 1 + (initialTime % 1000 - minefield->timer->remainingTime()) / 1000.0) + " seconds.");
        e->setWindowTitle("You Won!");
    } else {
        e->setWindowTitle("Game Over!");
    }
    connect(e, SIGNAL(requestQuit()), this, SLOT(quit()));
    connect(e, SIGNAL(requestMenu()), this, SLOT(showMenu()));
    connect(e, SIGNAL(requestMenu()), e, SLOT(close()));
    e->show();
}

void MainWindow::quit() {
    exit(0);
}

void MainWindow::enable() {
    setDisabled(false);
    minefield->timer->start();
}

void MainWindow::showMenu() {
    // Show menu dialog
    setDisabled(true);
    minefield->timer->stop();
    MenuDialog::disabled = MineButton::isFirst || isEnded;
    m = new MenuDialog(minefield->mineLCD->intValue(), MineButton::left, ((minefield->timeLCD->intValue() + 1) * 1000 + initialTime % 1000 - minefield->timer->remainingTime()), minefield->exportData());
    setWindowTitle("QtMine");
    connect(m, SIGNAL(rejected()), this, SLOT(enable()));
    connect(m, SIGNAL(requestShow()), this, SLOT(show()));
    connect(m, SIGNAL(requestShow(int, int, int, QBitArray *)), this, SLOT(show(int, int, int, QBitArray *)));
    m->exec();
    connect(minefield, SIGNAL(requestEnd(bool)), this, SLOT(showEnd(bool)));
}

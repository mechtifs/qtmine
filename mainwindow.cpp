#include "mainwindow.h"

int StartDialog::w;
int StartDialog::h;
int StartDialog::n;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    StartDialog::w = 20;
    StartDialog::h = 20;
    StartDialog::n = 50;
    start();
}

void MainWindow::start() {
    s = new StartDialog;
    setWindowTitle("QtMine");
    connect(s, SIGNAL(requestShow()), this, SLOT(show()));
    s->exec();
    connect(minefield, SIGNAL(requestEnd(bool)), this, SLOT(showEnd(bool)));
}

void MainWindow::show() {
    minefield = new MineField(StartDialog::w, StartDialog::h, StartDialog::n);
    setCentralWidget(minefield);
    resize(25 * StartDialog::w + 37, 25 * StartDialog::h + 93);
//    setMaximumSize(25 * StartDialog::w + 37, 25 * StartDialog::h + 93);
    connect(minefield, SIGNAL(requestRestart()), this, SLOT(restart()));
    QWidget::show();
}

void MainWindow::showEnd(bool hasWon) {
    e = new EndDialog;
    e->setModal(Qt::WindowModal);
    if (hasWon) {
        e->label->setText("Finished in " + QString::number(minefield->elapsedTimer->elapsed() / 1000.0) + " seconds.");
        e->setWindowTitle("You won!");
    } else {
        e->setWindowTitle("Game over!");
    }
    connect(e, SIGNAL(requestQuit()), this, SLOT(quit()));
    connect(e, SIGNAL(requestRestart()), e, SLOT(close()));
    connect(e, SIGNAL(requestRestart()), this, SLOT(restart()));
    e->show();
    setEnabled(false);
}

void MainWindow::quit() {
    exit(0);
}

void MainWindow::restart() {
    delete minefield;
    close();
    setEnabled(true);
    start();
}

#include "enddialog.h"

EndDialog::EndDialog() {
    newgame = new QPushButton("New Game");
    exit = new QPushButton("Quit");
    label = new QLabel("Have a retry?");
    vBox = new QVBoxLayout;
    hBox = new QHBoxLayout;
    hBox->addWidget(newgame);
    hBox->addWidget(exit);
    vBox->addWidget(label);
    vBox->addLayout(hBox, 1);
    setLayout(vBox);
    connect(newgame, SIGNAL(clicked()), this, SLOT(reject()));
    connect(newgame, SIGNAL(clicked()), this, SIGNAL(requestMenu()));
    connect(exit, SIGNAL(clicked()), this, SIGNAL(requestQuit()));
}

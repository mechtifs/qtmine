#include "enddiaalog.h"

EndDialog::EndDialog() {
    newgame = new QPushButton("New Game");
    exit = new QPushButton("Quit");
    label = new QLabel("Have a retry?");
    VBox = new QVBoxLayout;
    HBox = new QHBoxLayout;
    HBox->addWidget(newgame);
    HBox->addWidget(exit);
    VBox->addWidget(label);
    VBox->addLayout(HBox,1);
    setLayout(VBox);
    connect(newgame,SIGNAL(clicked()),this,SLOT(restart()));
    connect(exit,SIGNAL(clicked()),this,SLOT(quit()));
}

void EndDialog::restart() {
    emit requestRestart();
}

void EndDialog::quit(){
    emit requestQuit();
}

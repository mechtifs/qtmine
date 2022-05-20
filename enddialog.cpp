#include "enddialog.h"

EndDialog::EndDialog() {
    // Initialize buttons
    menuButton = new QPushButton("New Game");
    connect(menuButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(menuButton, SIGNAL(clicked()), this, SIGNAL(requestMenu()));
    quitButton = new QPushButton("Quit");
    connect(quitButton, SIGNAL(clicked()), this, SIGNAL(requestQuit()));
    // Initialize horizontal box layout
    hBox = new QHBoxLayout;
    hBox->addWidget(menuButton);
    hBox->addWidget(quitButton);
    // Initialize label
    label = new QLabel("Have a retry?");
    // Initialize vertical box layout
    vBox = new QVBoxLayout;
    vBox->addWidget(label);
    vBox->addLayout(hBox, 1);
    // Set main layout
    setLayout(vBox);
}

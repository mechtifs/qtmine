#include "menudialog.h"

MenuDialog::MenuDialog(int mines, int left, int time, QBitArray *mineData) : mines(mines), left(left), time(time), mineData(mineData) {
    setWindowTitle("Menu");
    // Initialize width spin box
    width = new QSpinBox;
    width->setMinimum(2);
    width->setMaximum(46340);
    width->setValue(w);
    connect(width, SIGNAL(valueChanged(int)), this, SLOT(onWidthChange()));
    // Initialize height spin box
    height = new QSpinBox;
    height->setMinimum(2);
    height->setMaximum(46340);
    height->setValue(h);
    connect(height, SIGNAL(valueChanged(int)), this, SLOT(onHeightChange()));
    // Initialize number spin box
    number = new QSpinBox;
    number->setMinimum(1);
    number->setMaximum(399);
    number->setValue(n);
    connect(number, SIGNAL(valueChanged(int)), this, SLOT(onNumberChange()));
    // Initialize form layout
    form = new QFormLayout;
    form->addRow("Row: ", height);
    form->addRow("Column: ", width);
    form->addRow("Number: ", number);
    form->setRowWrapPolicy(QFormLayout::WrapLongRows);
    // Initialize push buttons
    startButton = new QPushButton("Start");
    connect(startButton, SIGNAL(clicked()), this, SLOT(start()));
    saveButton = new QPushButton("Save");
    connect(saveButton, SIGNAL(clicked()), this, SLOT(onSave()));
    saveButton->setDisabled(disabled);
    loadButton = new QPushButton("Load");
    connect(loadButton, SIGNAL(clicked()), this, SLOT(onLoad()));
    quitButton = new QPushButton("Quit");
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));
    // Initialize horizontal box layout
    hBox = new QHBoxLayout;
    hBox->addWidget(startButton);
    hBox->addWidget(saveButton);
    hBox->addWidget(loadButton);
    hBox->addWidget(quitButton);
    // Initialize vertical box layout
    vBox = new QVBoxLayout;
    vBox->addLayout(form, 4);
    vBox->addLayout(hBox, 1);
    // Set main layout
    setLayout(vBox);
}

void MenuDialog::start() {
    emit requestShow();
    this->close();
}

void MenuDialog::quit() {
    exit(0);
}

void MenuDialog::onWidthChange() {
    // Update width value
    w = width->value();
    number->setMaximum(width->value() * height->value() - 1);
}

void MenuDialog::onHeightChange() {
    // Update height value
    h = height->value();
    number->setMaximum(width->value() * height->value() - 1);
}

void MenuDialog::onNumberChange() {
    // Update number value
    n = number->value();
}

void MenuDialog::onSave() {
    // Get file location
    QString saveName = QFileDialog::getSaveFileName(this, tr("Save"), "", tr("Save File (*.dat)"));
    if (saveName != "") {
        QFile save(saveName);
        save.open(QFile::WriteOnly);
        // Save data to file
        QDataStream stream(&save);
        stream << w << h << mines << left << time << *mineData;
        save.close();
    }
}

void MenuDialog::onLoad() {
    // Get file location
    QString saveName = QFileDialog::getOpenFileName(this, tr("Load"), "", tr("Save File (*.dat)"));
    if (saveName != "") {
        QFile save(saveName);
        save.open(QFile::ReadOnly);
        QDataStream stream(&save);
        // Read data from file
        int mines, left, time;
        QBitArray *mineData = new QBitArray(w * h * 3);
        stream >> w >> h >> mines >> left >> time >> *mineData;
        emit requestShow(mines, left, time, mineData);
        this->close();
    }
}

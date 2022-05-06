#include "menudialog.h"

MenuDialog::MenuDialog(int mines, int left, int time, QBitArray *saveData) : mines(mines), left(left), time(time), saveData(saveData) {
    setWindowTitle("Menu");
    vBox = new QVBoxLayout;
    form = new QFormLayout;
    width = new QSpinBox();
    width->setMinimum(2);
    width->setMaximum(46340);
    width->setValue(w);
    connect(width, SIGNAL(valueChanged(int)), this, SLOT(onWidthChange()));
    height = new QSpinBox();
    height->setMinimum(2);
    height->setMaximum(46340);
    height->setValue(h);
    connect(height, SIGNAL(valueChanged(int)), this, SLOT(onHeightChange()));
    number = new QSpinBox();
    number->setMinimum(1);
    number->setMaximum(399);
    number->setValue(n);
    connect(number, SIGNAL(valueChanged(int)), this, SLOT(onNumberChange()));
    form->addRow("Row: ", width);
    form->addRow("Column: ", height);
    form->addRow("Number: ", number);
    form->setRowWrapPolicy(QFormLayout::WrapLongRows);
    hBox = new QHBoxLayout;
    submit = new QPushButton("Start");
    cancel = new QPushButton("Quit");
    save = new QPushButton("Save");
    load = new QPushButton("Load");
    save->setDisabled(disabled);
    hBox->addWidget(submit);
    hBox->addWidget(save);
    hBox->addWidget(load);
    hBox->addWidget(cancel);
    vBox->addLayout(form, 4);
    vBox->addLayout(hBox, 1);
    setLayout(vBox);
    connect(submit, SIGNAL(clicked()), this, SLOT(start()));
    connect(load, SIGNAL(clicked()), this, SLOT(onLoad()));
    connect(save, SIGNAL(clicked()), this, SLOT(onSave()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(quit()));
}

void MenuDialog::start() {
    emit requestShow();
    this->close();
}

void MenuDialog::quit() {
    exit(0);
}

void MenuDialog::onWidthChange() {
    w = width->value();
    number->setMaximum(width->value() * height->value() - 1);
}

void MenuDialog::onHeightChange() {
    h = height->value();
    number->setMaximum(width->value() * height->value() - 1);
}

void MenuDialog::onNumberChange() {
    n = number->value();
}

void MenuDialog::onSave() {
    QString saveName = QFileDialog::getSaveFileName(this, tr("Save"), "", tr("Save File (*.dat)"));
    if (saveName != "") {
        QFile save(saveName);
        save.open(QFile::WriteOnly);
        QDataStream stream(&save);
//        qDebug() << w << h << mines << left << time << *saveData;
        stream << w << h << mines << left << time << *saveData;
        save.close();
    }
}

void MenuDialog::onLoad() {
    QString saveName = QFileDialog::getOpenFileName(this, tr("Load"), "", tr("Save File (*.dat)"));
    if (saveName != "") {
        QFile save(saveName);
        save.open(QFile::ReadOnly);
        QDataStream stream(&save);
        int mines, left, time;
        QBitArray *saveData = new QBitArray(w * h * 2);
        stream >> w >> h >> mines >> left >> time >> *saveData;
//        qDebug() << w << h << mines << left << time << *saveData;
        emit requestShow(mines, left, time, saveData);
        this->close();
    }
}

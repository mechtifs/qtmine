#include "startdialog.h"

StartDialog::StartDialog() {
    setWindowTitle("Set size");
    VBox = new QVBoxLayout;
    FormLayout = new QFormLayout;
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
    FormLayout->addRow("Row: ", width);
    FormLayout->addRow("Column: ", height);
    FormLayout->addRow("Number: ", number);
    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
    HBox = new QHBoxLayout;
    submit = new QPushButton("Start");
    cancel = new QPushButton("Quit");
    HBox->addWidget(submit);
    HBox->addWidget(cancel);
    VBox->addLayout(FormLayout, 4);
    VBox->addLayout(HBox, 1);
    setLayout(VBox);
    connect(submit, SIGNAL(clicked()), this, SLOT(start()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(quit()));
}

void StartDialog::start(){
    emit requestShow();
    this->close();
}

void StartDialog::quit(){
    exit(0);
}

void StartDialog::onWidthChange() {
    w = width->value();
//    height->setMaximum(2147483647 / w);
    number->setMaximum(width->value() * height->value() - 1);
}

void StartDialog::onHeightChange() {
    h = height->value();
//    width->setMaximum(2147483647 / h);
    number->setMaximum(width->value() * height->value() - 1);
}

void StartDialog::onNumberChange() {
    n = number->value();
}

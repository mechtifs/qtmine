#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QWidget>
#include <QSpinBox>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>


class StartDialog : public QDialog {
    Q_OBJECT
    QSpinBox * width;
    QSpinBox * height;
    QSpinBox * number;
    QPushButton * submit;
    QPushButton * cancel;
    QVBoxLayout * VBox;
    QFormLayout * FormLayout;
    QHBoxLayout * HBox;
public:
    static int w;
    static int h;
    static int n;
    StartDialog();
public slots:
    void start();
    void quit();
    void onWidthChange();
    void onHeightChange();
    void onNumberChange();
signals:
    void requestShow();
    void closeBox();
};


#endif // STARTDIALOG_H

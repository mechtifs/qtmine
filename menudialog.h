#ifndef MENUDIALOG_H
#define MENUDIALOG_H

#include <QWidget>
#include <QSpinBox>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QBitArray>
//#include <QDebug>


class MenuDialog : public QDialog {
    Q_OBJECT
    QSpinBox * width;
    QSpinBox * height;
    QSpinBox * number;
    QPushButton * submit;
    QPushButton * cancel;
    QPushButton * save;
    QPushButton * load;
    QVBoxLayout * vBox;
    QFormLayout * form;
    QHBoxLayout * hBox;
    int mines;
    int left;
    int time;
    QBitArray * saveData;
public:
    static int w;
    static int h;
    static int n;
    static bool disabled;
    MenuDialog(int mines = 0, int left = 0, int time = 0, QBitArray * saveData = NULL);
private slots:
    void onSave();
    void onLoad();
    void onWidthChange();
    void onHeightChange();
    void onNumberChange();
    void start();
    void quit();
signals:
    void requestShow();
    void requestShow(int mines, int left, int time, QBitArray * saveData);
    void closeBox();
};


#endif // MENUDIALOG_H

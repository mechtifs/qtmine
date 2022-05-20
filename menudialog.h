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

class MenuDialog : public QDialog {
    Q_OBJECT
    QSpinBox *width;
    QSpinBox *height;
    QSpinBox *number;
    QPushButton *startButton;
    QPushButton *quitButton;
    QPushButton *saveButton;
    QPushButton *loadButton;
    QVBoxLayout *vBox;
    QFormLayout *form;
    QHBoxLayout *hBox;
    QBitArray *mineData;
    int mines;
    int left;
    int time;
public:
    MenuDialog(int mines = 0, int left = 0, int time = 0, QBitArray *mineData = NULL);
    static int w;
    static int h;
    static int n;
    static bool disabled;
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
    void requestShow(int mines, int left, int time, QBitArray *mineData);
    void closeBox();
};

#endif // MENUDIALOG_H

#ifndef ENDDIALOG_H
#define ENDDIALOG_H

#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

class EndDialog : public QDialog {
    Q_OBJECT
    QPushButton *menuButton;
    QPushButton *quitButton;
    QVBoxLayout *vBox;
    QHBoxLayout *hBox;
public:
    EndDialog();
    QLabel *label;
signals:
    void requestMenu();
    void requestQuit();
};

#endif // ENDDIALOG_H

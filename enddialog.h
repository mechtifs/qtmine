#ifndef ENDDIAALOG_H
#define ENDDIAALOG_H

#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>


class EndDialog : public QDialog
{
    Q_OBJECT
    QPushButton *newgame;
    QPushButton *exit;
    QVBoxLayout *VBox;
    QHBoxLayout *HBox;
public:
    EndDialog();
    QLabel *label;
public slots:
    void restart();
    void quit();
signals:
    void requestRestart();
    void requestQuit();
};

#endif // ENDDIAALOG_H

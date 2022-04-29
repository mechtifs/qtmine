#include "minebutton.h"

MineButton::MineButton(int i, int j): x(i), y(j) {
    setFocusPolicy(Qt::NoFocus);
}

void MineButton::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        if (text() == "") {
            if (isFirst) {
                emit requestSetMines(x, y);
                emit requestFlatten(x, y);
                if (!left) {
                    emit requestEnd(true);
                }
                isFirst = false;
            } else if (hasMine) {
                emit requestEnd();
            } else {
                emit requestFlatten(x, y);
                if (!left) {
                    emit requestEnd(true);
                }
            }
        }
    } else if (e->button() == Qt::RightButton) {
        if (!isFirst) {
            flag();
        }
    }
}

void MineButton::flatten(int num) {
    if (text() == ""){
        setFlat(true);
        setDisabled(true);
    }
    if (num) {
        setText(QString::number(num));
    }
}

void MineButton::flag() {
    if (text() == "") {
        setText("*");
        emit requestFlag(x, y, 1);
    } else {
        setText("");
        emit requestFlag(x, y, -1);
    }
}

#include "minebutton.h"

MineButton::MineButton(int i, int j): x(i), y(j) {
    setFocusPolicy(Qt::NoFocus);
}

void MineButton::mouseReleaseEvent(QMouseEvent *e) {
    setDown(false);
    // Check button type
    if (e->button() == Qt::LeftButton) {
        if (!isFlagged()) {
            // Check if the button is clicked first
            if (isFirst) {
                emit requestSetMines(x, y);
                emit requestFlatten(x, y);
                // Case where there is only one safe button
                if (!left) {
                    emit requestEnd(true);
                }
                isFirst = false;
            } else if (hasMine) {
                // Game over
                emit requestEnd();
            } else {
                emit requestFlatten(x, y);
                // Check if the number of buttons left equals to mine count
                if (!left) {
                    emit requestEnd(true);
                }
            }
        }
    } else if (e->button() == Qt::RightButton) {
        // Disable right click when minefield is not yet generated
        if (!isFirst) {
            flag();
        }
    }
}

void MineButton::flatten(int num) {
    // Ignore if the button is flagged
    if (!isFlagged()) {
        setFlat(true);
        setDisabled(true);
    }
    // Display nearby mine count
    if (num) {
        setText(QString::number(num));
    }
}

void MineButton::flag() {
    // Switch state
    if (isFlagged()) {
        setText("");
        emit requestFlag(x, y, -1);
    } else {
        setText("*");
        emit requestFlag(x, y, 1);
    }
}

bool MineButton::isFlagged() {
    return text() == "*";
}

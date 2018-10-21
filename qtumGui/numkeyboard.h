#ifndef NUMKEYBOARD_H
#define NUMKEYBOARD_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class NumKeyBoard;
}

class NumKeyBoard : public QWidget
{
    Q_OBJECT

public:
    explicit NumKeyBoard(QWidget *parent = 0);
    ~NumKeyBoard();
     QString inputNum;
private:
    Ui::NumKeyBoard *ui;
    void keyPressEvent( QKeyEvent *k );
private slots:
    void  btnClicked();
signals:
    void sendsignal();
    void clickNum();
};

#endif // NUMKEYBOARD_H

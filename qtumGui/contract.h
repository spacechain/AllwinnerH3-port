#ifndef CONTRACT_H
#define CONTRACT_H

#include <QWidget>

namespace Ui {
class contract;
}

class contract : public QWidget
{
    Q_OBJECT

public:
    explicit contract(QWidget *parent = 0);
    ~contract();
signals:
    void sendsignal();
private:
    Ui::contract *ui;
    void keyPressEvent( QKeyEvent *k );
private slots :
   void returnFun();
};

#endif // CONTRACT_H

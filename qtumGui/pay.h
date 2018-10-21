#ifndef PAY_H
#define PAY_H

#include <QWidget>
#include "numkeyboard.h"
#include <QEvent>
#include <QDateTime>
#include "thread.h"
#include "dialog.h"
namespace Ui {
class pay;
}

class pay : public QWidget
{
    Q_OBJECT

public:
    explicit pay(QWidget *parent = 0);
    QDateTime       time;
    QString     Timestr;
    QFont font;
    QFont font2;
    Thread4   *thread4 = Thread4::GetInstance();
    Thread    *thread1 = Thread::GetInstance();
    Thread3    *thread3 = Thread3::GetInstance();
    Dialog    *moneyChanage = Dialog::GetInstance();
      bool flat  = true;
    ~pay();

signals:
    void sendsignal();
private:
    Ui::pay *ui;
    void keyPressEvent( QKeyEvent *k );

    void updatePosition();
    bool eventFilter(QObject *,QEvent *);    //注意这里软件盘相关
    NumKeyBoard      *input;


private slots :
   void returnFun();
   void moneypay();
   void reshow();
   void showNum();
   void scaning();
   void TimeView();
   void netOpen(qint8);
   void netOutTime();
   void balanceChange(double);
   void dialogClose();
};

#endif // PAY_H

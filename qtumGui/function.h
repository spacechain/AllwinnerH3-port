#ifndef FUNCTION_H
#define FUNCTION_H

#include <QWidget>
#include "recemoney.h"
#include "pay.h"
#include "contract.h"
#include "tradingrecord.h"
#include "thread.h"
#include <QTime>
#include <QTimer>
#include <QDateTime>
#include "dialog.h"
namespace Ui {
class function;
}

class function : public QWidget
{
    Q_OBJECT

public:
    explicit function(QWidget *parent = 0);
    receMoney      *rece;
    pay            *paymoney;
    contract       *contractMoney;
    tradingRecord  *record;
    Thread3        *thread3 =  Thread3::GetInstance();
    Thread         *thread1 =  Thread::GetInstance();
    Thread4        *thread4 =  Thread4::GetInstance();
    Dialog         *moneyChanage = Dialog::GetInstance();
    QDateTime       time;
    QString         Timestr;
    QFont           font;
    QFont           font2;
    ~function();
signals:
    void sendsignal();
private:
    Ui::function *ui;
    bool          flat2 = false;
    void keyPressEvent( QKeyEvent *k );


private slots:
    void clock_receiveMoney();
    void clock_moneypay();
    void clock_constract();
    void clock_record();
    void reshow();
    void returnFun();
    void balance();
    void processUI(qint32 num);
    void balanceChange(double);
    void TimeView();
    void netOpen(qint8);
    void netOutTime();
    void dialogClose();
};

#endif // FUNCTION_H

#ifndef TRADINGRECORD_H
#define TRADINGRECORD_H

#include <QWidget>
#include <QScrollBar>
#include <QStandardItem>
#include <QDateTime>
#include <QTimer>
#include "thread.h"
#include "dialog.h"
namespace Ui {
class tradingRecord;
}

class tradingRecord : public QWidget
{
    Q_OBJECT

public:
    explicit tradingRecord(QWidget *parent = 0);
    QDateTime time;
    QString   Timestr;
    QFont     font;
    QFont     font2;
    Thread4  *thread4 = Thread4::GetInstance();
    Thread   *thread1 = Thread::GetInstance();
    Thread3   *thread3 = Thread3::GetInstance();
    Dialog   *moneyChanage = Dialog::GetInstance();
    ~tradingRecord();

signals:
    void sendsignal();

private:
    Ui::tradingRecord *ui;
    bool flat;
    void keyPressEvent(QKeyEvent *k);

private slots :
    void returnFun();
    void see();
    void trableUp();
    void trableDown();
    void  TimeView();
    void netOpen(qint8);

    void netOutTime();
    void dialogClose();
    void balanceChange(double);
};

#endif // TRADINGRECORD_H

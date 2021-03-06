#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "function.h"
#include <QHostInfo>
#include <QDateTime>
#include <QThread>
#include"thread.h"
#include <QTimer>
namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    function *fun;
    QDateTime time;
    QString   Timestr;
    QFont     font;
    QFont     font2;
    Thread   *thread1 = Thread::GetInstance();
    Thread4  *thread4 = Thread4::GetInstance();
    Thread2  thread2;
    Thread3  *thread3 = Thread3::GetInstance();
    QTimer   *timer3;
    bool      flat =0;
    ~Widget();
private:
    Ui::Widget *ui;
    void keyPressEvent( QKeyEvent *k );
private slots:
    void  pushButton_true();
    void  password();
    void  reshow();
    void  closeQtum();
    void  TimeView();
    void  netOpen(qint8);
    void  netOutTime();
    void  qtumup();
    void  qtumShowdown();
};

#endif // WIDGET_H

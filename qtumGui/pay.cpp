#include "pay.h"
#include "ui_pay.h"
#include <QKeyEvent>
#include <QDebug>
#include <QProcess>
#include <QEvent>
#include <QTimer>
#include "thread.h"
#define   PRECONTROL    123
#define   NEXTCONTROL   125
#define   TRUECLICK     43

pay::pay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pay)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(240,320);
    connect(ui->pushButton_return,SIGNAL(clicked(bool)),SLOT(returnFun()));
    connect(ui->pushButton_pay,SIGNAL(clicked(bool)),this,SLOT(moneypay()));
    connect(ui->pushButton_2Code,SIGNAL(clicked(bool)),this,SLOT(scaning()));
    connect(thread1,SIGNAL(netchange(qint8)),this,SLOT(netOpen(qint8)));
    connect(thread4,SIGNAL(timechange()),this,SLOT(TimeView()));
    connect(thread3, SIGNAL(moneychange(double)), this, SLOT(balanceChange(double)));

    ui->pushButton_return->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_pay->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_2Code->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->label_net->setAlignment(Qt::AlignCenter);
    ui->lineEdit_money->installEventFilter(this);
    ui->lineEdit->setFocusPolicy(Qt::NoFocus);
    font2.setFamily("宋体");
    font2.setPixelSize(15);
    ui->label_Timer->setFont(font2);
    ui->label_net->setFont(font2);
    QTimer *timer2 = new QTimer(this);
    connect(timer2,SIGNAL(timeout()),this,SLOT(netOutTime()));
    timer2->start(1000*8);

}

pay::~pay()
{
    delete ui;
}
void pay::returnFun()
{
    emit sendsignal();
    this->close();
}
void pay::moneypay()
{
   QProcess process;

   QString cmd = "/apps/qtum/qtum-cli -testnet sendtoaddress";
   cmd = cmd + " " + ui->lineEdit->text() + "  " + ui->lineEdit_money->text();

   process.start(cmd);
   process.waitForStarted(5000);
   process.waitForFinished(10000);
   ui->lineEdit_money->clear();
}
void pay::keyPressEvent( QKeyEvent *k )
{
    if(k->key() == PRECONTROL) {
      this->focusNextPrevChild(false);
    }
    if(k->key() == NEXTCONTROL) {
      this->focusNextPrevChild(true);
    }
    if(k->key() == TRUECLICK) {
       QWidget *current_focus_w = this->focusWidget();                                   /* 获取当前焦点的控件 */
       if(!current_focus_w)
           return;
       QPushButton * btn = qobject_cast<QPushButton*>(current_focus_w);
       if(btn)
           btn->click();
    }
}

bool pay::eventFilter(QObject *watched, QEvent *event)
{
    if (watched==ui->lineEdit_money)                                                   /* 首先判断控件(这里指 lineEdit_money)         */
    {
        if (event->type()==QEvent::FocusIn)                                            /* 然后再判断控件的具体事件 (这里指获得焦点事件)   */
        {   input=new NumKeyBoard();
            updatePosition();
            connect(input,SIGNAL(sendsignal()),this,SLOT(reshow()));
            connect(input,SIGNAL(clickNum()),this,SLOT(showNum()));
            input->show();
            ui->lineEdit->clearFocus();                                               /* 防止weightMaxlineEdit重新获得焦点再一次激发时间  */
        }
    }

    return QWidget::eventFilter(watched,event);                                      // 最后将事件交给上层对话框
}
void pay::updatePosition()
{
    QWidget *widget = focusWidget();
    if (!widget) {
        return;
    }

    QRect widgetRect = widget->rect();
    QPoint panelPos = QPoint(widgetRect.left(), widgetRect.bottom() + 2);
    panelPos = widget->mapToGlobal(panelPos);
    input->move(panelPos);
}
void pay::reshow()
{
     ui->pushButton_pay->setFocus();
     ui->lineEdit_money->setText(input->inputNum);
}
void pay::showNum()
{
   ui->lineEdit_money->setText(input->inputNum);
}

void pay::scaning()
{
    QProcess process;

    process.start("/apps/ttyS1Test/ttyS1Test");
    process.waitForStarted(5000);
    process.waitForFinished(10000);
    QByteArray byteArray = process.readAllStandardOutput();
    QString str = QString::fromLocal8Bit(byteArray);

    ui->lineEdit->setText(str);
}
void pay:: TimeView()
{

    time = QDateTime::currentDateTime();
    Timestr = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->label_Timer->setText(Timestr);

}
void pay::netOpen(qint8 state)
{
    if(state == 1) {
        flat = true;
        ui->label_net->setStyleSheet("background-color:green");
        ui->label_net->setText("已连接");
    } else {
        ui->label_net->setAlignment(Qt::AlignCenter);
        ui->label_net->setStyleSheet("background-color:red");
        ui->label_net->setText("断开");
    }
}
void pay::netOutTime()
{
    if(flat == true) {
        ui->label_net->setStyleSheet("background-color:green");
        ui->label_net->setText("已连接");
        flat = false;
    } else {
        ui->label_net->setStyleSheet("background-color:red");
        ui->label_net->setText("断开");
    }
}
void pay::dialogClose()
{
    moneyChanage->close();
}
void pay::balanceChange(double money)
{
    moneyChanage->show();
    QTimer::singleShot(3000, this, SLOT(dialogClose()));

}

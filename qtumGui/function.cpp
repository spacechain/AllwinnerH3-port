#include "function.h"
#include "ui_function.h"
#include <QProcess>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include "thread.h"
#include "dialog.h"
#define   PRECONTROL    123
#define   NEXTCONTROL   125
#define   TRUECLICK     43

function::function(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::function)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(240,320);

    connect(ui->pushButton_receMoney, SIGNAL(clicked(bool)), this, SLOT(clock_receiveMoney()));
    connect(ui->pushButton_pay, SIGNAL(clicked(bool)), this, SLOT(clock_moneypay()));
    connect(ui->pushButton_contract, SIGNAL(clicked(bool)), this, SLOT(clock_constract()));
    connect(ui->pushButton_record, SIGNAL(clicked(bool)), this, SLOT(clock_record()));
    connect(ui->pushButton_return, SIGNAL(clicked(bool)), this, SLOT(returnFun()));
    connect(ui->pushButton_balance, SIGNAL(clicked(bool)), this, SLOT(balance()));
    connect(thread3, SIGNAL(moneychange(double)), this, SLOT(balanceChange(double)));
    connect(thread3,SIGNAL(processNum(qint32)),this,SLOT(processUI(qint32)));
    connect(thread1,SIGNAL(netchange(qint8)),this,SLOT(netOpen(qint8)));
    connect(thread4,SIGNAL(timechange()),this,SLOT(TimeView()));

    ui->pushButton_receMoney->setStyleSheet("QPushButton{background-color: rgb(0, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_pay->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_contract->setStyleSheet("QPushButton{background-color: rgb(255, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_record->setStyleSheet("QPushButton{background-color: rgb(255, 170, 127);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_return->setStyleSheet("QPushButton{background-color: rgb(85, 255, 127);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_balance->setStyleSheet("QPushButton{background-color: rgb(85, 170, 127);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");

    QTimer *timer2 = new QTimer(this);
    connect(timer2,SIGNAL(timeout()),this,SLOT(netOutTime()));
    timer2->start(1000*11);


    font.setFamily("宋体");
    font.setPixelSize(25);
    font2.setFamily("宋体");
    font2.setPixelSize(15);
    ui->label_Time->setFont(font2);
    ui->label_net->setFont(font2);
    ui->label_viewBalance->setFont(font);
    ui->progressBar->setRange(0,1000);
    ui->label_net->setAlignment(Qt::AlignCenter);
    ui->progressBar->setValue(0);
    ui->progressBar->setFormat(QString::fromLocal8Bit("qtum同步数据进度：%p%"));

    contractMoney = new contract();

    ui->label_viewBalance->setFont(font);
//    ui->label_viewBalance->setText("同步数据到99%显示实时金额");
}

function::~function()
{
    delete ui;
}
void function::clock_receiveMoney()
{
   rece =new receMoney();
   rece->show();
   connect(rece, SIGNAL(sendsignal()), this, SLOT(reshow()));
   this->hide();
}
void function::clock_moneypay()
{
    paymoney = new pay();
    paymoney->show();
    connect(paymoney, SIGNAL(sendsignal()), this, SLOT(reshow()));
    this->hide();
}
void function::reshow()
{
    this->show();
}
void function::clock_constract()
{
    contractMoney->show();
    this->hide();
    connect(contractMoney, SIGNAL(sendsignal()), this, SLOT(reshow()));
}
void function::clock_record()
{
    record = new tradingRecord();
    record->show();
    this->hide();
    connect(record, SIGNAL(sendsignal()), this, SLOT(reshow()));
}
void function::returnFun()
{
    emit sendsignal();
    this->close();
}
void function::balance()
{
    QProcess     process;
    QByteArray   byteArray;
    QString      str;

    process.start("/apps/qtum/qtum-cli -testnet getbalance");
    process.waitForStarted(5000);
    process.waitForFinished(10000);
    byteArray = process.readAllStandardOutput();
    str = QString::fromLocal8Bit(byteArray);
    process.close();
    ui->label_viewBalance->setText(str);
}
void function::keyPressEvent( QKeyEvent *k )
{
    if(k->key() == PRECONTROL) {
      this->focusNextPrevChild(false);
    }
    if(k->key() == NEXTCONTROL) {
      this->focusNextPrevChild(true);
    }
    if(k->key() == TRUECLICK) {
       QWidget *currentFocusW = this->focusWidget();
       if(!currentFocusW) {
           return;
       }
       QPushButton * btn = qobject_cast<QPushButton*>(currentFocusW);
       if(btn) {
           btn->click();
       }
    }
}
void function::processUI(qint32 num)
{
    ui->progressBar->setValue(num);
    return;
}
void  function::balanceChange(double money)
{
    moneyChanage->show();
    QTimer::singleShot(3000, this, SLOT(dialogClose()));
}
void function::TimeView()
{
    time = QDateTime::currentDateTime();
    Timestr = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->label_Time->setText(Timestr);
}
void function::netOpen(qint8 state)
{
    if(state == 1) {
        flat2 = true;
        ui->label_net->setStyleSheet("background-color:green");
        ui->label_net->setText("已连接");

    } else {
        ui->label_net->setFont(font2);
        ui->label_net->setAlignment(Qt::AlignCenter);
        ui->label_net->setStyleSheet("background-color:red");
        ui->label_net->setText("断开");
    }
}
void function::netOutTime()
{

    if(flat2 == true) {
        ui->label_net->setStyleSheet("background-color:green");
        ui->label_net->setText("已连接");
        flat2 = false;
    } else {
        ui->label_net->setStyleSheet("background-color:red");
        ui->label_net->setText("断开");
    }
}
void function::dialogClose()
{
    moneyChanage->close();
}

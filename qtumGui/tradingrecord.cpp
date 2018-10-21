#include "tradingrecord.h"
#include "ui_tradingrecord.h"
#include <QProcess>
#include <QDebug>
#include <QKeyEvent>
#include <qjson4/QJsonArray.h>
#include <qjson4/QJsonDocument.h>
#include <qjson4/QJsonObject.h>
#include <qjson4/QJsonParseError.h>
#include <qjson4/QJsonParser.h>
#include <qjson4/QJsonRoot.h>
#include <qjson4/QJsonValue.h>
#include <qjson4/QJsonValueRef.h>
#include <QStandardItemModel>
#include <QPoint>

#define   PRECONTROL    123
#define   NEXTCONTROL   125
#define   TRUECLICK     43

tradingRecord::tradingRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tradingRecord)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(240, 320);

    ui->pushButton_down->setStyleSheet("QPushButton{background-color: rgb(255, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_up->setStyleSheet("QPushButton{background-color: rgb(255, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_return->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_see->setStyleSheet("QPushButton{background-color: rgb(85, 255, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_see->setFocus();
    ui->tableView->setFocusPolicy(Qt::NoFocus);
    connect(thread3, SIGNAL(moneychange(double)), this, SLOT(balanceChange(double)));
    connect(ui->pushButton_return,SIGNAL(clicked(bool)),SLOT(returnFun()));
    connect(ui->pushButton_see,SIGNAL(clicked(bool)),SLOT(see()));
    connect(ui->pushButton_down,SIGNAL(clicked(bool)),this, SLOT(trableDown()));
    connect(ui->pushButton_up,SIGNAL(clicked(bool)),this,SLOT(trableUp()));
    connect(thread4,SIGNAL(timechange()),this,SLOT(TimeView()));
    connect(thread1,SIGNAL(netchange(qint8)),this,SLOT(netOpen(qint8)));

   font.setFamily("宋体");
   font.setPixelSize(25);
   font2.setFamily("宋体");
   font2.setPixelSize(15);
   ui->label_Time->setFont(font2);

   QTimer *timer2 = new QTimer(this);
   connect(timer2,SIGNAL(timeout()),this,SLOT(netOutTime()));
   timer2->start(1000*9);
   ui->label_net->setFont(font2);
}

tradingRecord::~tradingRecord()
{
    delete ui;
}
void tradingRecord::returnFun()
{
    emit sendsignal();
    this->close();

}
void tradingRecord::see() {
    QProcess                  process;
    QJsonParseError           error;
    QString                   strMoney;
    QString                   nodeNum;
    INT                       NNum;
    QByteArray                byteArray;
    QJsonDocument             doc ;
    QStandardItemModel       *model;
    QStandardItem            *blue;
    QStandardItem            *red;
    QStandardItem            *green;

    model = new QStandardItemModel();
    green = new QStandardItem(QIcon(":/icon/E:/project/H3/source/QRcTest/image/p.png"),"确认");
    red = new QStandardItem(QIcon(":/icon/E:/project/H3/source/QRcTest/image/red.png"),"失败");
    blue = new QStandardItem(QIcon(":/icon/E:/project/H3/source/QRcTest/image/blue.png"),"合并");
    model->setColumnCount(4);

    model->setHeaderData(0,Qt::Horizontal, "状态");
    model->setHeaderData(1,Qt::Horizontal, "节点");
    model->setHeaderData(2,Qt::Horizontal, "方式");
    model->setHeaderData(3,Qt::Horizontal, "金额");
//    model->setRowCount(3);
//     model->setItem(0,0,blue);
//      model->setItem(1,0,red);
//       model->setItem(2,0,green);
    process.start("/apps/qtum/qtum-cli -testnet listtransactions");
    process.waitForStarted(5000);
    process.waitForFinished(15000);

    byteArray = process.readAllStandardOutput();
    doc = QJsonDocument::fromJson(byteArray, &error);

    if(!doc.isNull() && error.error == QJsonParseError::NoError) {
        QJsonArray array = doc.array();
        model->setRowCount(array.size());

        for(int i=0; i< array.size(); i++) {
            QJsonObject object = array.at(i).toObject();  // 转化为对象
            strMoney = QString::number(object.take("amount").toDouble());
            NNum  = object.take("confirmations").toInt();
            green = new QStandardItem(QIcon(":/icon/E:/project/H3/source/QRcTest/image/p.png"),"确认");
            red = new QStandardItem(QIcon(":/icon/E:/project/H3/source/QRcTest/image/red.png"),"失败");
            blue = new QStandardItem(QIcon(":/icon/E:/project/H3/source/QRcTest/image/blue.png"),"合并");
           if( NNum > 500) {
             model->setItem(i,0,blue);
         } else if(NNum > 1){
                model->setItem(i,0,green);
           } else {
               model->setItem(i,0,red);
           }
           nodeNum = QString::number(NNum);
            model->setItem(i, 1, new QStandardItem(nodeNum));
            model->setItem(i, 2, new QStandardItem(object.take("category").toString()));
            model->setItem(i,3, new QStandardItem(strMoney));
        }

     }
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0,60);
    ui->tableView->setColumnWidth(1,36);
    ui->tableView->setColumnWidth(2,60);
    ui->tableView->setColumnWidth(3,47);
    process.close();

}
void tradingRecord::keyPressEvent( QKeyEvent *k ) {
    if(k->key() == PRECONTROL) {
        this->focusNextPrevChild(false);//按A时焦点切换至上一部件
    }
    if(k->key() == NEXTCONTROL) {
        this->focusNextPrevChild(true);//按D时焦点切换至下一部件
    }
    if(k->key() == TRUECLICK) {
        QWidget *current_focus_w = this->focusWidget();//获取当前焦点的控件
        if(!current_focus_w) {
           return;
        }
        QPushButton * btn = qobject_cast<QPushButton*>(current_focus_w);
        if(btn) {
           btn->click();
        }
    }
}
void tradingRecord::trableDown()
{
    int a= ui->tableView->verticalScrollBar()->value();
    ui->tableView->verticalScrollBar()->setValue(a+2);

}
void tradingRecord::trableUp()
{
    int a= ui->tableView->verticalScrollBar()->value();
    ui->tableView->verticalScrollBar()->setValue(a-2);

}

void tradingRecord:: TimeView()
{

    time = QDateTime::currentDateTime();
    Timestr = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->label_Time->setText(Timestr);

}
void tradingRecord::netOpen(qint8 state)
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
void tradingRecord::netOutTime()
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
void tradingRecord::dialogClose()
{
    moneyChanage->close();
}
void  tradingRecord::balanceChange(double money)
{
    moneyChanage->show();
    QTimer::singleShot(3000, this, SLOT(dialogClose()));

}

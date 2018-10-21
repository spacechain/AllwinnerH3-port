#include "widget.h"
#include "ui_widget.h"
#include "function.h"
#include <QPushButton>
#include <QFont>
#include <QDebug>
#include <QKeyEvent>
#include <QProcess>
#include <QNetworkConfigurationManager>
#include <QTimer>
#include <thread.h>
#define   PRECONTROL    123
#define   NEXTCONTROL   125
#define   TRUECLICK     43

QString input;
INT      process;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(240,320);

    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);
    ui->horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
    ui->horizontalLayout_3->setContentsMargins(0, 0, 0 ,0);
    ui->horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
    ui->verticalLayout_5->setContentsMargins(0, 0, 0 ,0);

    ui->pushButton_num_0->setContentsMargins(0, 0, 0, 0);
    ui->pushButton_num_1->setContentsMargins(0, 0, 0, 0);
    ui->pushButton_num_2->setContentsMargins(0, 0 ,0 ,0);
    ui->pushButton_num_3->setContentsMargins(0, 0, 0, 0);
    ui->pushButton_num_4->setContentsMargins(0, 0, 0, 0);
    ui->pushButton_num_5->setContentsMargins(0, 0, 0, 0);
    ui->pushButton_num_6->setContentsMargins(0, 0, 0, 0);
    ui->pushButton_num_7->setContentsMargins(0, 0, 0, 0);
    ui->pushButton_num_8->setContentsMargins(0, 0, 0, 0);
    ui->pushButton_num_9->setContentsMargins(0, 0, 0, 0);
    ui->pushButton_num_10->setContentsMargins(0, 0, 0, 0);
    ui->pushButton_num_11->setContentsMargins(0, 0, 0, 0);

    ui->pushButton_num_0->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_num_1->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_num_2->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_num_3->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_num_4->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_num_5->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_num_6->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_num_7->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_num_8->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_num_9->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_num_10->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_num_11->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_false->setStyleSheet("QPushButton{background-color: rgb(255, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_true->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_close_qtum->setStyleSheet("QPushButton{background-color: rgb(185, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->label_view->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    ui->label_view->clear();

    connect(ui->pushButton_true, SIGNAL(clicked(bool)), this, SLOT(pushButton_true()));
    connect(ui->pushButton_num_0, SIGNAL(clicked(bool)), this, SLOT(password()));
    connect(ui->pushButton_num_1, SIGNAL(clicked(bool)), this, SLOT(password()));
    connect(ui->pushButton_num_2, SIGNAL(clicked(bool)), this, SLOT(password()));
    connect(ui->pushButton_num_3, SIGNAL(clicked(bool)), this, SLOT(password()));
    connect(ui->pushButton_num_4, SIGNAL(clicked(bool)), this, SLOT(password()));
    connect(ui->pushButton_num_5, SIGNAL(clicked(bool)), this, SLOT(password()));
    connect(ui->pushButton_num_6, SIGNAL(clicked(bool)), this, SLOT(password()));
    connect(ui->pushButton_num_7, SIGNAL(clicked(bool)), this, SLOT(password()));
    connect(ui->pushButton_num_8, SIGNAL(clicked(bool)), this, SLOT(password()));
    connect(ui->pushButton_num_9, SIGNAL(clicked(bool)), this, SLOT(password()));
    connect(ui->pushButton_num_10, SIGNAL(clicked(bool)), this, SLOT(password()));
    connect(ui->pushButton_num_11, SIGNAL(clicked(bool)), this, SLOT(password()));
    connect(ui->pushButton_false, SIGNAL(clicked(bool)), this, SLOT(password()));
    connect(ui->pushButton_close_qtum, SIGNAL(clicked(bool)), this ,SLOT(closeQtum()));
    connect(thread1,SIGNAL(netchange(qint8)),this,SLOT(netOpen(qint8)));
    connect(thread4,SIGNAL(timechange()),this,SLOT(TimeView()));

    font.setFamily("宋体");
    font.setPixelSize(50);
    font2.setFamily("宋体");
    font2.setPixelSize(15);
    ui->label_view->setFont(font);

    QTimer *timer1 = new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(netOutTime()));
    timer1->start(1000*10);

    QTimer *timer2 = new QTimer(this);
    connect(timer2,SIGNAL(timeout()),this,SLOT(qtumShowdown()));
    timer2->start(1000*60*35);

    timer3 = new QTimer(this);
    connect(timer3,SIGNAL(timeout()),this,SLOT(qtumup()));
    timer3->start(1000*10);

    ui->label_net->setAlignment(Qt::AlignCenter);
    ui->label->setFont(font2);
    ui->label_net->setText("检测中.....");
    ui->label_Time->setFont(font2);
    ui->label_net->setFont(font2);
    fun =new function();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::reshow() {
    this->show();
}

void Widget::pushButton_true()
{
   if(!input.compare("12")) {
        input.clear();
        ui->label_view->clear();

        connect(fun,SIGNAL(sendsignal()), this, SLOT(reshow()));
        fun->show();
        this->hide();
   } else {
       input.clear();
       ui->label_view->clear();
   }
}
void Widget::password() {

      QPushButton *currentButton=qobject_cast<QPushButton*>(this->sender());
      if(currentButton)
      {
          if(currentButton == ui->pushButton_num_0) {
              input.append('0');
              ui->label_view->setText(input);
          }
          if(currentButton == ui->pushButton_num_1) {
               input.append('1');
               ui->label_view->setText(input);
          }
          if(currentButton == ui->pushButton_num_2) {
               input.append('2');
               ui->label_view->setText(input);
          }
          if(currentButton == ui->pushButton_num_3) {
               input.append('3');
               ui->label_view->setText(input);
          }
          if(currentButton == ui->pushButton_num_4) {
               input.append('4');
               ui->label_view->setText(input);
          }
          if(currentButton == ui->pushButton_num_5) {
              input.append('5');
              ui->label_view->setText(input);
          }

          if(currentButton == ui->pushButton_num_6) {
              input.append('6');
              ui->label_view->setText(input);
          }
          if(currentButton == ui->pushButton_num_7) {
              input.append('7');
              ui->label_view->setText(input);
          }
          if(currentButton==ui->pushButton_num_8) {
              input.append('8');
              ui->label_view->setText(input);
          }
          if(currentButton == ui->pushButton_num_9) {
             input.append('9');
             ui->label_view->setText(input);
          }
          if(currentButton==ui->pushButton_num_10) {
             input.append('*');
             ui->label_view->setText(input);
          }
          if(currentButton==ui->pushButton_num_11) {
             input.append('#');
             ui->label_view->setText(input);
          }
          if(currentButton==ui->pushButton_false) {
             input.clear();
             ui->label_view->setText(input);
          }
      }
}
void Widget::keyPressEvent( QKeyEvent *k )
{
    if(k->key() == PRECONTROL) {
      this->focusNextPrevChild(false);
    }
    if(k->key() == NEXTCONTROL)
    {
      this->focusNextPrevChild(true);
    }

    if(k->key() == TRUECLICK) {
       QWidget *current_focus_w = this->focusWidget();
       if(!current_focus_w) {
           return;
       }
       QPushButton * btn = qobject_cast<QPushButton*>(current_focus_w);
       if(btn) {
           btn->click();
       }

    }

}

void Widget::closeQtum()
{
    QProcess process;
    process.start("/apps/qtum/qtum-cli -testnet stop");
    process.waitForStarted(5000);
    process.waitForFinished(10000);
}
void Widget:: TimeView()
{
    time = QDateTime::currentDateTime();
    Timestr = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->label_Time->setText(Timestr);
}

void Widget::netOpen(qint8 state)
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
void Widget::netOutTime()
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
void Widget::qtumup()
{
    if(Timestr.startsWith("1970",Qt::CaseSensitive))  {
        return;
    } else {
        timer3->stop();
        QProcess qtum;
        qtum.startDetached("/apps/qtum/qtumd -testnet -printtoconsole");
        thread3->start();
    }

}
void Widget::qtumShowdown()
{
    thread2.start();
}


#include "frmnum.h"
#include "ui_frmnum.h"
#include "qdebug.h"
#include <QKeyEvent>
#include <QDebug>
frmNum::frmNum(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmNum)
{
    ui->setupUi(this);
    this->InitForm();//初始化窗体数据


    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    QList<QPushButton *> btn = this->findChildren<QPushButton *>();
    foreach (QPushButton * b, btn) {
        connect(b, SIGNAL(clicked()), this, SLOT(btn_clicked()));
    }

    ui->btn0->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->btn1->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");

}

frmNum::~frmNum()
{
    delete ui;
}





void frmNum::btn_clicked()
{

}


void frmNum::keyPressEvent( QKeyEvent *k )
{
    qDebug()<<"frm"<<k->key();
    if(k->key() == 123)
    {
      this->focusNextPrevChild(false);//按A时焦点切换至上一部件
    }
    if(k->key() == 125)
    {
      this->focusNextPrevChild(true);//按D时焦点切换至下一部件
    }

    if(k->key() == 43) {

       QWidget *current_focus_w = this->focusWidget();//获取当前焦点的控件
       if(!current_focus_w)
           return;
       QPushButton * btn = qobject_cast<QPushButton*>(current_focus_w);
       if(btn)
           btn->click();

    }

}

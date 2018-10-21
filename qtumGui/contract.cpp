#include "contract.h"
#include "ui_contract.h"
#include <QDebug>
#include <QKeyEvent>

#define   PRECONTROL    123
#define   NEXTCONTROL   125
#define   TRUECLICK     43

contract::contract(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::contract)
{
    ui->setupUi(this);
    connect(ui->pushButton_return, SIGNAL(clicked(bool)), SLOT(returnFun()));
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(240, 320);

}

contract::~contract()
{
    delete ui;
}
void contract::returnFun()
{
    emit sendsignal();
    this->close();
}
void contract::keyPressEvent( QKeyEvent *k )
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

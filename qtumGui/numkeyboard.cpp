#include "numkeyboard.h"
#include "ui_numkeyboard.h"
#include <QDebug>
#define   PRECONTROL    123
#define   NEXTCONTROL   125
#define   TRUECLICK     43
NumKeyBoard::NumKeyBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NumKeyBoard)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->pushButton_0->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_1->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_2->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_3->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_4->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_5->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_6->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_7->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_8->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_9->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_clear->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_dou->setStyleSheet("QPushButton{border: 2px solid white;}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_detele->setStyleSheet("QPushButton{background-color: rgb(255, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton_true->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");

    QList<QPushButton *> btn = this->findChildren<QPushButton *>();
    foreach (QPushButton * b, btn) {
        connect(b, SIGNAL(clicked()), this, SLOT(btnClicked()));
    }
}

NumKeyBoard::~NumKeyBoard()
{
    delete ui;
}

void NumKeyBoard::keyPressEvent( QKeyEvent *k )
{
    if(k->key() == PRECONTROL) {
      this->focusNextPrevChild(false);                  /*  按A时焦点切换至上一部件         */
    }
    if(k->key() == NEXTCONTROL) {
      this->focusNextPrevChild(true);                  /*   按D时焦点切换至下一部件         */
    }
    if(k->key() == TRUECLICK) {
       QWidget *currenFtocusW = this->focusWidget();
       if(!currenFtocusW) {
           return;
       }
       QPushButton * btn = qobject_cast<QPushButton*>(currenFtocusW);
       if(btn) {
           btn->click();
       }
    }

}
void NumKeyBoard::btnClicked()
{
    QPushButton *currentButton=qobject_cast<QPushButton*>(this->sender());
    if(currentButton)  {
        if(currentButton == ui->pushButton_0) {
            inputNum.append('0');
            emit clickNum();
        }
        if(currentButton == ui->pushButton_1) {
             inputNum.append('1');
             emit clickNum();
        }
        if(currentButton == ui->pushButton_2) {
             inputNum.append('2');
             emit clickNum();
        }
        if(currentButton == ui->pushButton_3) {
             inputNum.append('3');
             emit clickNum();

        }
        if(currentButton == ui->pushButton_4)  {
             inputNum.append('4');
             emit clickNum();

        }
        if(currentButton == ui->pushButton_5)  {
            inputNum.append('5');
            emit clickNum();
        }

        if(currentButton == ui->pushButton_6)  {
            inputNum.append('6');
            emit clickNum();
        }
        if(currentButton == ui->pushButton_7) {
            inputNum.append('7');
            emit clickNum();
        }
        if(currentButton == ui->pushButton_8) {
            inputNum.append('8');
            emit clickNum();
        }
        if(currentButton == ui->pushButton_9) {
           inputNum.append('9');
           emit clickNum();
        }
        if(currentButton == ui->pushButton_dou) {
            if(inputNum.isEmpty() == true) {
                return;
            }
            if(inputNum.contains('.') == true) {
                return;
            }
           inputNum.append('.');
           emit clickNum();
        }
        if(currentButton == ui->pushButton_detele) {
           inputNum.chop(1);
           emit clickNum();
        }
        if(currentButton==ui->pushButton_clear) {
           inputNum.clear();
           emit clickNum();
        }
        if (currentButton == ui->pushButton_true) {
            emit sendsignal();
            this->close();
        }
    }

}



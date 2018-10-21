#include "dialog.h"
#include "ui_dialog.h"
#include <QTimer>
#include <QDebug>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->move(50,100);

}

Dialog::~Dialog()
{
    delete ui;
}


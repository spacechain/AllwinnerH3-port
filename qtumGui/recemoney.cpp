#include "recemoney.h"
#include "ui_recemoney.h"
#include <QProcess>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#define   PRECONTROL    123
#define   NEXTCONTROL   125
#define   TRUECLICK     43

receMoney::receMoney(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::receMoney)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(240,320);

    ui->pushButton_return->setStyleSheet("QPushButton{background-color: rgb(85, 170, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton->setStyleSheet("QPushButton{background-color: rgb(85, 255, 0);}QPushButton:focus{ border-color: rgb(0, 160, 230); background: rgb(85, 85, 85); }");
    ui->pushButton->setFocus();
    ui->lineEdit->setFocusPolicy(Qt::NoFocus);

    connect(ui->pushButton_return, SIGNAL(clicked(bool)), SLOT(returnFun()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(GenerationTwoCode()));
    connect(thread1,SIGNAL(netchange(qint8)),this,SLOT(netOpen(qint8)));
    connect(thread4,SIGNAL(timechange()),this,SLOT(TimeView()));
    connect(thread3, SIGNAL(moneychange(double)), this, SLOT(balanceChange(double)));

    QProcess process;
    process.start("/apps/qtum/qtum-cli -testnet getnewaddress");
    process.waitForStarted(5000);
    process.waitForFinished(10000);

    QByteArray byteArray = process.readAllStandardOutput();
    QString str = QString::fromLocal8Bit(byteArray);
    ui->lineEdit->setText(str);
    process.close();
}

receMoney::~receMoney()
{
    delete ui;
}
void receMoney::returnFun() {
    emit sendsignal();
    this->close();
}
QPixmap receMoney::createQRCode(const QString &text)
{
    int margin = 2;
    if (text.length() == 0){
        return QPixmap();
    }
    QRcode *qrcode = QRcode_encodeString(text.toLocal8Bit(), 2, QR_ECLEVEL_L, QR_MODE_8, 0);
    if (qrcode == NULL) {
        return QPixmap();
    }
    unsigned char *p, *q;
    p = NULL;
    q = NULL;
    int x, y, bit;
    int realwidth;

    realwidth = qrcode->width;
    QImage image = QImage(realwidth, realwidth, QImage::Format_Indexed8);
    QRgb value;
    value = qRgb(255,255,255);
    image.setColor(0, value);
    value = qRgb(0,0,0);
    image.setColor(1, value);
    image.setColor(2, value);
    image.fill(0);
    p = qrcode->data;
    for(y=0; y<qrcode->width; y++) {
        bit = 7;
        q += margin / 8;
        bit = 7 - (margin % 8);
        for(x=0; x<qrcode->width; x++)  {
            if ((*p & 1) << bit)
                image.setPixel(x, y, 1);
            else
                image.setPixel(x, y, 0);
            bit--;
            if(bit < 0){
                q++;
                bit = 7;
            }
            p++;
        }
    }
    return QPixmap::fromImage(image.scaledToWidth(200));
}
void receMoney::GenerationTwoCode() {
    QString text = ui->lineEdit->text();
    ui->label->setFixedSize(200, 200);
    ui->label->setPixmap(createQRCode(text));
}
void receMoney::keyPressEvent( QKeyEvent *k )
{
    if(k->key() == PRECONTROL) {
      this->focusNextPrevChild(false);
    }
    if(k->key() == NEXTCONTROL) {
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
void receMoney:: TimeView()
{
    time = QDateTime::currentDateTime();
    Timestr = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->label_Timer->setText(Timestr);
}

void receMoney::netOpen(qint8 state)
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
void receMoney::netOutTime()
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
void receMoney::dialogClose()
{
    moneyChanage->close();
}
void  receMoney::balanceChange(double money)
{
    moneyChanage->show();
    QTimer::singleShot(3000, this, SLOT(dialogClose()));

}

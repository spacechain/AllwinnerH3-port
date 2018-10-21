#include "thread.h"
#include <QDebug>
#include <QNetworkConfigurationManager>
#include <QProcess>
#include "widget.h"
#include <qjson4/QJsonArray.h>
#include <qjson4/QJsonDocument.h>
#include <qjson4/QJsonObject.h>
#include <qjson4/QJsonParseError.h>
#include <qjson4/QJsonParser.h>
#include <qjson4/QJsonRoot.h>
#include <qjson4/QJsonValue.h>
#include <qjson4/QJsonValueRef.h>
Thread::Thread()
{

}
void Thread::run()
{
   QHostInfo host;
   while(1) {
       host = QHostInfo::fromName("114.114.114.114");
       if (host.error() != QHostInfo::NoError) {
           emit netchange(0);
       } else {
             emit netchange(1);
       }
       Thread::msleep(1000*5);
   }

}

Thread2::Thread2()
{

}
void Thread2::run()
{

    QProcess                  process;
    QByteArray                byteArray;
    QJsonDocument             doc ;
    QJsonParseError           error;
    QJsonValue                value;
    QJsonObject               object;
    double                    NNum = 0;

     process.start("/apps/qtum/qtum-cli -testnet  getblockchaininfo");
     process.waitForStarted(5000);
     process.waitForFinished(10000);

     byteArray = process.readAllStandardOutput();
     doc = QJsonDocument::fromJson(byteArray, &error);

     if(!doc.isNull() && error.error == QJsonParseError::NoError) {
           object = doc.object();  // 转化为对象
           value = object.value("verificationprogress");
           NNum = value.toDouble();

            if( NNum > 0.96) {
                process.close();
                process.startDetached("/apps/qtum/qtum-cli -testnet stop");
                process.waitForStarted(5000);
                process.waitForFinished(10000);
                process.close();
                Thread2::msleep(1000*60*2);
                process.startDetached("/apps/qtum/qtumd -testnet -printtoconsole");
          }

     } else {
        process.close();
     }
}
Thread3::Thread3()
{

}
void Thread3::run()
{

    QProcess                  process;
    QByteArray                byteArray;
    QJsonDocument             doc ;
    QJsonParseError           error;
    QJsonValue                value;
    QJsonObject               object;
    QString                   moneystr;
    double                    NNum = 0;
    qint32                    procNum;
    double                    lastmoney = 0;
    double                    money = 0;
   while(1) {
       process.start("/apps/qtum/qtum-cli -testnet  getblockchaininfo");
       process.waitForStarted(5000);
       process.waitForFinished(5000);

       byteArray = process.readAllStandardOutput();
       doc = QJsonDocument::fromJson(byteArray, &error);

       if(!doc.isNull() && error.error == QJsonParseError::NoError) {
             object = doc.object();  // 转化为对象
             value = object.value("verificationprogress");
             NNum = value.toDouble();
             procNum = (qint32)(NNum *1000);
             emit processNum(procNum);
       }
       process.close();
       process.start("/apps/qtum/qtum-cli -testnet getbalance");
       process.waitForStarted(5000);
       process.waitForFinished(6000);
       byteArray = process.readAllStandardOutput();
       moneystr = QString::fromLocal8Bit(byteArray);
       money = moneystr.toDouble();
       process.close();
       if(money != lastmoney ) {
       emit moneychange(money);
       }
       if( NNum > 0.96) {
       Thread3::msleep(1000*60*2);
       } else {
            Thread3::msleep(1000*60);
       }

  }

}


Thread4::Thread4()
{

}
void Thread4::run()
{
   while(1) {
       emit timechange();
       Thread4::msleep(1000);
   }

}

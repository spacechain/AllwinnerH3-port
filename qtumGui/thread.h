#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QHostInfo>
#include <QMetaType>
class Thread :public QThread
{
  Q_OBJECT

private:
    Thread();

public:
    virtual void run();
    void stop();
    static Thread * GetInstance()
        {
            static Thread *Instance;
            if(Instance == NULL) //判断是否第一次调用
                Instance = new Thread();
            return Instance;
        }


signals:
    void netchange(qint8);
public  slots:

};

class Thread2 :public QThread
{
  Q_OBJECT

public:
    Thread2();
    virtual void run();
    void stop();

private:


};

class Thread3 :public QThread
{
  Q_OBJECT

private:
    Thread3();
public:
    static Thread3 * GetInstance()
        {
            static Thread3 *Instance;
            if(Instance == NULL) //判断是否第一次调用
                Instance = new Thread3();
            return Instance;
        }
    virtual void run();
    void stop();
signals:
    void processNum(qint32);
    void  moneychange(double);
private:
};

class Thread4 :public QThread
{
  Q_OBJECT

private:
    Thread4();

public:
    static Thread4 * GetInstance()
        {
            static Thread4 *m_pInstance;
            if(m_pInstance == NULL) //判断是否第一次调用
                m_pInstance = new Thread4();
            return m_pInstance;
        }

    virtual void run();
    void stop();
signals:

 void  timechange();
private:

};

#endif // THREAD_H

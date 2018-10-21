#ifndef FRMNUM_H
#define FRMNUM_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QPushButton>
#include <QTimer>
#include <QEvent>

namespace Ui
{
    class frmNum;
}

class frmNum : public QWidget
{
    Q_OBJECT

public:
    explicit frmNum(QWidget *parent = 0);
    ~frmNum();

    

protected:

private slots:
  
    //输入法面板按键处理
    void btn_clicked();


private:
    Ui::frmNum *ui;
   

    void keyPressEvent( QKeyEvent *k );

};

#endif // FRMNUM_H

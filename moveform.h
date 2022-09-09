#ifndef MOVEFORM_H
#define MOVEFORM_H
#include <QWidget>
#include<QChart>
#include<QChartView>
#include<QTreeWidgetItem>
#include "movecalculate.h"
#include "udpsocketclient.h"
QT_CHARTS_USE_NAMESPACE   //使用QChart必须要添加这句
#include<moveconstant.h>
namespace Ui {
class moveform;
}

class moveform : public QWidget
{
    Q_OBJECT

public:
    explicit moveform(QWidget *parent = nullptr);
    ~moveform();
    void createform();
    void getinfor(QString qtime,double qspeed,QString wind,double wave);
    static MOVETYPE movetype;

private:
    Ui::moveform *ui;
    QTimer *timer = new QTimer(this);
    QTimer *timer1=new QTimer(this);
    MoveCalculate* cal;
    UdpSocketClient m_udpclient;
    MOVETYPE premovetype=None;

public slots:
    void timerUpdate(); //显示时间函数
    void treeWidgetUpdate();
    void chartViewUpdate(QVector<float>data);


private slots:
    void on_drawBtn_clicked();
    void on_calBtn_clicked();
    void on_CalBtn_clicked();

    void on_DrawBtn_clicked();
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
};
#endif // MOVEFORM_H

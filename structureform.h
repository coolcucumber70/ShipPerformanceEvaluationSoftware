#ifndef STRUCTUREFORM_H
#define STRUCTUREFORM_H
#include <QWidget>
#include<QChart>
#include<QChartView>
#include<QThread>
#include "Network/SocketClient.h"
#include "sqlwork/CSQLDatabase.h"
#include<vector>
using namespace std;
QT_CHARTS_USE_NAMESPACE   //使用QChart必须要添加这句
namespace Ui {
class structureform;
}

class structureform : public QWidget
{
    Q_OBJECT

public:
    explicit structureform(QWidget *parent = nullptr);
    ~structureform();
    SocketClient *m_socketClient;
    int StressIndex=0;
signals:
    void connectTcpServer();
    void sendmsg();

private slots:
    void on_pushButton_connect_clicked();
    void dataPacketReady(SocketPacket packet);
    void socketConnected(bool value);
    void on_toolBox_currentChanged(int index);


private:
    Ui::structureform *ui;
};

#endif // STRUCTUREFORM_H

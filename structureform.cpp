#include "structureform.h"
#include "ui_structureform.h"
#include "Network/SocketClient.h"
#include <QMessageBox>
#include "QSimpleLed.h"

structureform::structureform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::structureform)
{
    ui->setupUi(this);
    m_socketClient= new SocketClient;
    QThread *Thread1 = new QThread;
    m_socketClient->moveToThread(Thread1);
    QString sql="INSERT INTO tb_data (time,data1,data2,data3,data4) VALUES (?,?,?,?,?)";
    QThread *Thread2 = new QThread;
    connect(this,&structureform::connectTcpServer,m_socketClient,&SocketClient::connectTcpServer);
    connect(m_socketClient, &SocketClient::dataPacketReady,
            this, &structureform::dataPacketReady);
    //connect(m_socketClient, &SocketClient::dataPacketReady,
            //m_insertsql, &InsertSql::dataPacketReady);
    connect(m_socketClient, &SocketClient::socketConnected,
            this, &structureform::socketConnected);
    connect(this,&structureform::sendmsg,m_socketClient,&SocketClient::sendmsg);
    Thread1->start();
    Thread2->start();
   }

structureform::~structureform()
{
    delete ui;
}

void structureform::on_pushButton_connect_clicked()
{
    bool connected = m_socketClient->isConnected();
    if(!connected)
    {
        emit connectTcpServer();
    }
    else
    {
        QMessageBox::information(this, "提示", "已连接。");
    }

}

void structureform::dataPacketReady(SocketPacket packet)
{
//    DHPacket dp;
//    dp.read(packet.data);
//    QVector<float>v=dp.data[StressIndex];
//    ui->StructView->updateseries(v,0);




}

void structureform::socketConnected(bool value)
{

    QString text = tr("连接状态：");
    if(value == true)
    {
        text += QString("<font color=blue>%1</font>").arg(tr("已连接"));
    }
    else
    {
        text += QString("<font color=red>%1</font>").arg(tr("未连接"));
    }

    ui->label_network_status->setText(text);


}

void structureform::on_toolBox_currentChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

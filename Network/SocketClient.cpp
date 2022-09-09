#include "SocketClient.h"
#include "Constant.h"
#include "MyDebug.h"
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <QThread>
#define PACKET_SIZE  (1024 * 500)

SocketClient::SocketClient()
{
    m_dataSocket  = NULL;

    m_isConnecting = true;

    //connect(&m_diagnoseTimer, SIGNAL(timeout()),
            //this, SLOT(socketTimeOut()));
}

//使用单例模式
//SocketClient *SocketClient::getInstance()
//{
//    static SocketClient instance; //局部静态变量，若定义为指针，则需要手动释放内容
//    return &instance;
//}

bool SocketClient::isConnected()
{
    MY_DEBUG("");
    if(m_dataSocket == NULL || (m_dataSocket !=  NULL &&
        m_dataSocket->state() !=  QTcpSocket::ConnectedState))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void SocketClient::connectTcpServer()
{
    QTcpSocket *socket = new QTcpSocket(this);
    m_tcpServerIP = "127.0.0.1";
    m_tcpServerPort = 10002;
    socket->connectToHost(m_tcpServerIP, m_tcpServerPort);
    socket->waitForConnected();
    if(socket->state() == QTcpSocket::ConnectedState)
    {
        if(m_dataSocket != NULL)
        {
            m_dataSocket->abort();
            delete m_dataSocket;
        }

        m_dataSocket = socket;
        connect(m_dataSocket, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
        connect(m_dataSocket, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));

        emit socketConnected(true);
    }
    else
    {
        delete socket;
        emit socketConnected(false);
    }
}

void SocketClient::OnReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());//这里是直接就能够找到socket了。

    //qDebug()<<"start OnReadyRead"<<QThread::currentThreadId()<<endl;
    if(socket == m_dataSocket )
    {
        m_dataPacket.data += socket->readAll();
        MY_LOG("socket connection successful");
        //QString s=m_dataPacket.data.toHex();
        //qDebug()<<s<<endl;
        // 循环解析包数据，m_dataPacket.data 中可能不只一包数据
        bool isOk = false;
        do{
            isOk = parsePacket(socket, &m_dataPacket);
        }while(isOk);
    }
    else
    {
        MY_LOG("socket connection abnormal");
    }


}

// 解包
bool SocketClient::parsePacket(QTcpSocket *socket, SocketPacket *packet)
{
    int pIndexStart = packet->data.indexOf(NET_PACKET_START);
    if(pIndexStart < 0)
    {
        return false;//false代买数据是缺失的，是有问题的，放弃对这次数据的提取，等待下一次的数据接收
    }
    packet->data = packet->data.mid(pIndexStart); //截取从包头index_start到末尾的数据m_dataPacket
    SocketPacket tmpPacket;
    tmpPacket.data = packet->data;
    tmpPacket.data.remove(0, NET_PACKET_START_BYTES);//删除包头

    //解析包长度
    if(tmpPacket.data.count() < NET_PACKET_LTNGTH_BYTES)
    {
        return false;
    }

    QByteArray size=tmpPacket.data.mid(0, NET_PACKET_LTNGTH_BYTES);
    short len=size[1]&0x00FF;
    len|=(size[0]<<8)&0xFF00;
    tmpPacket.length =len;
    qDebug()<<"length is "<<len<<endl;
    if(tmpPacket.length+NET_PACKET_LTNGTH_BYTES+NET_PACKET_CHECK_BYTES<tmpPacket.data.size()){
        return false;
    }
    //数据到达包长度
    tmpPacket.data.remove(0, NET_PACKET_LTNGTH_BYTES);//删除数据长度
    if(tmpPacket.length+NET_PACKET_CHECK_BYTES > tmpPacket.data.count())
    {
        return false;
    }
    tmpPacket.data.resize(tmpPacket.length+NET_PACKET_CHECK_BYTES);//删除多余数据
    char checksum=0x00;
    for(int i=0;i<tmpPacket.length;i++){
        checksum+=tmpPacket.data.at(i);
    }
    checksum=~checksum;
    qDebug()<<"check is "<<checksum<<endl;
    //qDebug()<<" the true check is"<<tmpPacket.data.back()<<endl;
//    if(tmpPacket.data.back()!=checksum)
//    {
//        qDebug()<<" gogo true check is"<<tmpPacket.data.back()<<endl;
//        packet->data.remove(0, NET_PACKET_START_BYTES);//删除包头
//        if(packet->data.indexOf(NET_PACKET_START) >= 0)
//        {
//            return true;//有可能出现粘包的情况，继续解析后面数据
//        }
//        else
//        {
//            return false;
//        }
//    }
    tmpPacket.data.resize(tmpPacket.length); //删除包尾

    //解析出数据类型
    if(tmpPacket.data.count() < NET_PACKET_TYPE_BYTES)
    {
        return false;
    }
    QByteArray dataType = tmpPacket.data.left(NET_PACKET_TYPE_BYTES);
    tmpPacket.dataType = dataType;
    qDebug()<<"datatype is ="<<dataType<<endl;
    tmpPacket.data.remove(0, NET_PACKET_TYPE_BYTES);//删除数据类型
    //发送数据包消息
//    QString s=tmpPacket.data.toHex();
//    qDebug()<<s<<endl;

    if(socket == m_dataSocket)
    {
        emit dataPacketReady(tmpPacket);
    }
    packet->data.remove(0,
                        NET_PACKET_START_BYTES
                        + NET_PACKET_LTNGTH_BYTES
                        + tmpPacket.length+NET_PACKET_CHECK_BYTES);

    return true;
}

void SocketClient::OnDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());

    if(m_dataSocket !=  NULL)
    {
        m_dataSocket->abort();
        m_dataSocket = NULL;
    }

    m_diagnoseTimer.stop();

    emit socketConnected(false);

    MY_DEBUG("");
}

//封包并发送
void SocketClient::sendmsg()
{
    QTcpSocket *socket = m_dataSocket;
    if(socket == NULL || (socket !=  NULL &&
       socket->state() !=  QTcpSocket::ConnectedState))
    {
        return;
    }
    QByteArray data="5A AA 55 5A 6B 00 00 00 00 00 00 00";
    QByteArray packet = QByteArray::fromHex(data);
    socket->write(packet);
}

void SocketClient::socketTimeOut()
{
    MY_DEBUG("socketTimeOut !");
    m_diagnoseTimer.stop();
    OnDisconnected();
}






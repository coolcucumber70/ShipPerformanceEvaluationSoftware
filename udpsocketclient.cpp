#include "udpsocketclient.h"
#include    <Network/Constant.h>
UdpSocketClient::UdpSocketClient(QObject *parent) : QObject(parent)
{
    m_udpSocket=new QUdpSocket(this);//用于与连接的客户端通讯的QTcpSocket

}

bool UdpSocketClient::isConnected()
{
    if(m_udpSocket == NULL || (m_udpSocket !=  NULL &&
            m_udpSocket->state() !=  QAbstractSocket::ConnectedState))
        {
            return false;
        }
        else
        {
            return true;
        }

}

void UdpSocketClient::workstart()
{
    QString     IP="";
    groupAddress=QHostAddress(IP);//多播组地址
    quint16     groupPort=0;
    m_udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption,1);
    m_udpSocket->bind(QHostAddress::AnyIPv4, groupPort, QUdpSocket::ShareAddress);//先绑定端口
    m_udpSocket->joinMulticastGroup(groupAddress); //加入多播组
    connect(m_udpSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this,SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    onSocketStateChange(m_udpSocket->state());

    connect(m_udpSocket,SIGNAL(readyRead()),
                this,SLOT(onSocketReadyRead()));




}

void UdpSocketClient::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
    emit socketConnected(socketState);

}

void UdpSocketClient::onSocketReadyRead()
{

    while(m_udpSocket->hasPendingDatagrams())
        {
            SocketPacket packet;
            packet.length=m_udpSocket->pendingDatagramSize();
            packet.data.resize(packet.length);
            m_udpSocket->readDatagram(packet.data.data(),packet.length);
            emit dataPacketReady(packet);

        }
}

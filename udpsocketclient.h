#ifndef UDPSOCKETCLIENT_H
#define UDPSOCKETCLIENT_H
#include<Network/SocketConstant.h>
#include <QObject>
#include    <QUdpSocket>
class UdpSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit UdpSocketClient(QObject *parent = 0);
    QUdpSocket  *m_udpSocket;//用于与连接的客户端通讯的QTcpSocket
    QHostAddress    groupAddress;//组播地址
    bool isConnected();


signals:
    void socketConnected(QAbstractSocket::SocketState socketState);
    void dataPacketReady(SocketPacket socketData);

public slots:
    void workstart();
    void    onSocketStateChange(QAbstractSocket::SocketState socketState);
    void onSocketReadyRead();
};

#endif // UDPSOCKETCLIENT_H

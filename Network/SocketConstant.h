/***********************************************************
**
** 作者：lth
** 日期：2016-03-09
** 描述：定义网络通讯中用到的常量、数据结构
**
************************************************************/
#ifndef SOCKETCONSTANT
#define SOCKETCONSTANT
#include <QByteArray>
#include <QList>
#include <QIODevice>
#include <QDataStream>
#include "Constant.h"
#include<QtGlobal>
#include <QMetaType>
//Socket连接参数
#define NET_TCP_IP             "127.0.0.1"
#define NET_TCP_PORT             10002    //TCP服务器监听“起始”端口号
#define NET_PORT_SPACE           10      //尝试监听、绑定、广播端口号间隔

#define NET_SOCKET_TIMEOUT       10*1000 //多长时间没有收到客户端信息说明已经断开
#define NET_SHAKE_HAND_ENABLED   false


//包数据结构 : [ 包头 + 长度(2字节) + 包类型(1字节) + 数据 + 校验位 ]
#define NET_PACKET_START                0x7E
#define NET_PACKET_START_BYTES          1
#define NET_PACKET_TYPE_BYTES           1    //包类型占几个字节
#define NET_PACKET_LTNGTH_BYTES         2    //包数据长度占几个字节
#define NET_PACKET_CHECK_BYTES          1    //包数据检验位占几个字节

//包类型
#define NET_PACKET_TYPE_CMD                "01" //命令
#define NET_PACKET_TYPE_LED_CONTROL        "02" //LED 灯控制
#define NET_PACKET_TYPE_SYS_STATUS         "03" //系统状态
#define NET_PACKET_TYPE_TEXT               "04" //文本

//控制命令
#define NET_CMD_GET_SYS_STATUS           "GET_SYS_STATUS" //获取系统状态

//用于存储接收到的数据包
class  SocketPacket
{
public:
    short       length;   //数据长度
    QByteArray data;     //数据
    QByteArray dataType; //数据类型

    SocketPacket()
    {
        qRegisterMetaType<SocketPacket>("SocketPacket");
        length = INIT_NUMBER;
    }

    void clear()
    {
        length = INIT_NUMBER;
        data.clear();
        dataType.clear();
    }
};

#endif // SOCKETCONSTANT


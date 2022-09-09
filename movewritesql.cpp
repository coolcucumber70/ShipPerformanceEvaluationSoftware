#include "movewritesql.h"
#include<Network/SocketConstant.h>
MoveWriteSql::MoveWriteSql(QObject *parent) : QObject(parent)
{

}

void MoveWriteSql::dataPacketReady(SocketPacket packet)
{
    QString sql;
    QVector<QVector<float>>data;
//    switch(packet.dataType){
//    case 1:
//        //emit sendseries(1);
//    case 2:
//        //emit sendseries(2);
//    case 3:
//    case 4:
//    case 5:
//    case 6:
//    default:
//    };
    glSqlDatabse.ExecuteStmt(sql,data);
}

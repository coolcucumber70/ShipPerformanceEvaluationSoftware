#ifndef MOVEWRITESQL_H
#define MOVEWRITESQL_H

#include <QObject>
#include<Network/SocketConstant.h>
#include "sqlwork/CSQLDatabase.h"
class MoveWriteSql : public QObject
{
    Q_OBJECT
public:
    explicit MoveWriteSql(QObject *parent = 0);

signals:
    void sendseries(QVector<float>data);

public slots:
    void dataPacketReady(SocketPacket packet);
};

#endif // MOVEWRITESQL_H

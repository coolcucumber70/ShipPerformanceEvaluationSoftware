#include "qtosql.h"
#include<QDebug>
#include<QVector>

QToSql::QToSql(QObject *parent) : QObject(parent)
{

}

void QToSql::connectsql(QString name,QString connection)
{
     db = QSqlDatabase::addDatabase("QODBC",connection);
     db.setDatabaseName(name);
     if (db.open())   //打开数据库
     {
         qDebug()<<"打开数据库成功";
         emit ConnectOk();
     }
}
void QToSql::recdata(int *data, int count)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QSqlQuery query(db);
    int a[260];
    for(int i=0;i<260;i++)
    {
        a[i]=data[i];
    }
    query.prepare("insert into [school].[dbo].[mytest] ([dTime],[sensor1],[sensor2],[sensor3],[sensor4],[sensor5],[sensor6],[sensor7],[sensor8],[sensor9],[sensor10],[sensor11],[sensor12],[sensor13]) "
                      "values (:dTime, :sensor1, :sensor2, :sensor3, :sensor4, :sensor5, :sensor6, :sensor7, :sensor8, :sensor9, :sensor10, :sensor11, :sensor12, :sensor13)");
    for(int i=0;i<20;i++)
    {
        query.bindValue(":dTime", time);
        query.bindValue(":sensor1", a[i]);
        query.bindValue(":sensor2", a[20+i]);
        query.bindValue(":sensor3", a[40+i]);
        query.bindValue(":sensor4", a[60+i]);
        query.bindValue(":sensor5", a[80+i]);
        query.bindValue(":sensor6", a[100+i]);
        query.bindValue(":sensor7", a[120+i]);
        query.bindValue(":sensor8", a[140+i]);
        query.bindValue(":sensor9", a[160+i]);
        query.bindValue(":sensor10", a[180+i]);
        query.bindValue(":sensor11", a[200+i]);
        query.bindValue(":sensor12", a[220+i]);
        query.bindValue(":sensor13", a[240+i]);
        query.exec();

    }

}

#ifndef QTOSQL_H
#define QTOSQL_H
#include <QObject>
#include    <QtSql>
#include <QSqlQuery>
#include<QVector>


class QToSql : public QObject
{
    Q_OBJECT
public:
    explicit QToSql(QObject *parent = nullptr);
    void connectsql(QString name,QString connection);
    void recdata(int *data, int count);
    QSqlDatabase db;
    QSqlQueryModel model;

signals:
    void ConnectOk();
    void sendmodel(QSqlQueryModel*q);
public slots:

};

#endif // QTOSQL_H

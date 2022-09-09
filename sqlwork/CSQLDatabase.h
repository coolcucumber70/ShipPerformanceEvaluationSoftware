#ifndef CSQLDATABASE_H
#define CSQLDATABASE_H

#include <QtSql>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QQueue>
#include<QVector>
using namespace std;
/*
 * qt时间样式
 * 年月：yyyy-mm-dd
 * 时间：hh24:mi:ss
 * 年月时间：yyyy-mm-dd hh24:mi:ss
 */


class CSQLDatabase
{
public:
    /*
     * 单例模式获取
     * 参数：ip，用户名，用户密码，数据库名，数据库类型，端口号，最大连接数，最长等待时间，等待间隔
     */
    static CSQLDatabase& GetInstance(QString qsHostName, QString qsUser, QString qsPassWd,
                                     QString qsDatabase, QString qsDatabaseType, int nPort,
                                     int nMaxConn, int nMaxWaitTime, int nWaitInter);

    bool                ExecuteStmt(QString qsSql); //用于增删改
    bool                ExecuteStmt(QString qsSql,QVector<QVector<float>>data);
    QSqlQuery           ExecuteQuery(QString qsSql);//用于查询

    ~CSQLDatabase();

private:
    CSQLDatabase(QString qsHostName, QString qsUser, QString qsPassWd,
                 QString qsDatabase, QString qsDatabaseType, int nPort,
                 int nMaxConn, int nMaxWaitTime, int nWaitInter);

    QSqlDatabase    OpenConnection();
    void            CloseConnection(QSqlDatabase connection);
    QSqlDatabase    CreateConnect(const QString& qsConnName);
private:
    int m_nPort;
    int m_nMaxConn;
    int m_nMaxWaitTime; //最长等待时间
    int m_nWaitInter;   //等待间隔

    QString m_qsHostName;
    QString m_qsUser;
    QString m_qsPassWd;
    QString m_qsDatabase;
    QString m_qsDatabaseType;


    QQueue<QString> m_queUseConnNames;
    QQueue<QString> m_queUnuserConnNames;

    static QMutex m_Mutex;
    static QWaitCondition m_WaitConn;
    static CSQLDatabase* m_pInstance;
};

static CSQLDatabase glSqlDatabse = CSQLDatabase::GetInstance("127.0.0.1", "root", "Wfp5763020000", "test", "QMYSQL", 3306, 10, 10, 1);

#endif // CSQLDATABASE_H


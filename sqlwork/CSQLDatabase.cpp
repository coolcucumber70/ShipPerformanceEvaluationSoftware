#include "CSQLDatabase.h"

QMutex CSQLDatabase::m_Mutex;
CSQLDatabase* CSQLDatabase::m_pInstance = nullptr;
QWaitCondition CSQLDatabase::m_WaitConn;

CSQLDatabase& CSQLDatabase::GetInstance(QString qsHostName, QString qsUser, QString qsPassWd,
                                 QString qsDatabase, QString qsDatabaseType, int nPort,
                                 int nMaxConn, int nMaxWaitTime, int nWaitInter)
{
    if(m_pInstance == nullptr)
    {
        QMutexLocker locker(&m_Mutex);
        m_pInstance = new CSQLDatabase(qsHostName, qsUser, qsPassWd, qsDatabase,
                                       qsDatabaseType, nPort, nMaxConn, nMaxWaitTime, nWaitInter);
    }

    return *m_pInstance;
}

CSQLDatabase::CSQLDatabase(QString qsHostName, QString qsUser, QString qsPassWd,
                           QString qsDatabase, QString qsDatabaseType, int nPort,
                           int nMaxConn, int nMaxWaitTime, int nWaitInter)
    :m_qsHostName(qsHostName), m_qsUser(qsUser), m_qsPassWd(qsPassWd), m_qsDatabase(qsDatabase),
      m_qsDatabaseType(qsDatabaseType), m_nPort(nPort), m_nMaxConn(nMaxConn),
      m_nMaxWaitTime(nMaxWaitTime), m_nWaitInter(nWaitInter)
{
}

CSQLDatabase::~CSQLDatabase()
{
    QMutexLocker locker(&m_Mutex);
    foreach(QString connName, m_pInstance->m_queUseConnNames)   //移除所有使用中的连接
        QSqlDatabase::removeDatabase(connName);

    foreach(QString unConnName, m_pInstance->m_queUnuserConnNames)  //移除所有未使用的连接
        QSqlDatabase::removeDatabase(unConnName);

    m_pInstance = nullptr;
}

QSqlDatabase CSQLDatabase::OpenConnection()
{
    QMutexLocker locker(&m_Mutex);
    QString qsConnName;

    int nCountT = m_queUseConnNames.size() + m_queUnuserConnNames.size();
    //如果未到等待时间，并且所有的连接都在使用中，根据等待间隔进入到等待
    for(int i = 0; i < m_nMaxWaitTime && m_queUnuserConnNames.size() == 0 && nCountT == m_queUseConnNames.size(); i += m_nWaitInter)
    {
        //进行等待
        m_WaitConn.wait(&m_Mutex, m_nWaitInter);
        //重新计数总连接
        nCountT = m_queUseConnNames.size() + m_queUnuserConnNames.size();
    }

    //判断是否需要创建新连接，使用未使用连接，或者在连接全在使用时返回空的连接（可自己修改）
    if(m_queUnuserConnNames.size() > 0)
        qsConnName = m_queUnuserConnNames.dequeue();
    else if(nCountT < m_nMaxConn)
        qsConnName = QString("Connection-%1").arg(nCountT + 1);
    else {
        qDebug() << "All use sql connect";
        return QSqlDatabase();
    }

    //创建新连接，并放入到正在使用的连接容器中
    QSqlDatabase db = CreateConnect(qsConnName);
    if(db.isOpen())
        m_queUseConnNames.enqueue(qsConnName);

    return db;
}

void CSQLDatabase::CloseConnection(QSqlDatabase connection)
{
    //关闭连接，并将连接名放回未连接容器中
    QMutexLocker locker(&m_Mutex);
    QString qsConnName = connection.connectionName();
    if(m_queUseConnNames.contains(qsConnName))
    {
        m_queUseConnNames.removeOne(qsConnName);
        m_queUnuserConnNames.enqueue(qsConnName);
        //如果某一线程已使用完连接，另一线程正在申请一个数据库连接，进行唤醒操作
        m_WaitConn.wakeOne();
    }
}

QSqlDatabase CSQLDatabase::CreateConnect(const QString &qsConnName)
{
    //查看该连接名是否已经创建过连接
    if(QSqlDatabase::contains(qsConnName))
    {
        QSqlDatabase db = QSqlDatabase::database(qsConnName);
        return db;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(m_qsDatabaseType, qsConnName);
    db.setHostName(m_qsHostName);
    db.setDatabaseName(m_qsDatabase);
    db.setUserName(m_qsUser);
    db.setPassword(m_qsPassWd);
    db.setPort(m_nPort);

    if(!db.open())
    {
        qDebug() << "Open sql error" << db.lastError().text();
        return QSqlDatabase();
    }

    return db;
}
bool CSQLDatabase::ExecuteStmt(QString qsSql)
{
    QSqlDatabase db = OpenConnection();
    if(!db.isOpen())
        return false;

    QSqlQuery query = db.exec(qsSql);
    CloseConnection(db);

    if(query.lastError().isValid())
    {
        qDebug() << "Sql error:" << query.lastError();
        return false;
    }

    return true;
}

bool CSQLDatabase::ExecuteStmt(QString qsSql, QVector<QVector<float> > data)
{
    QSqlDatabase db = OpenConnection();
    if(!db.isOpen())
        return false;
    QSqlQuery query(db);
    query.prepare(qsSql);
    QDateTime dateTime;
    dateTime = QDateTime::currentDateTime();
    //qDebug() << dateTime.toString("yyyy-MM-dd hh:mm:ss ddd");
    int m=data[0].size();
    int n=data.size();
    for(int i=0;i<m;i++)
    {
        query.bindValue(0,dateTime);
        for(int j=0;j<n;j++){
            query.bindValue(j+1, data[j][i]);
        }
        query.exec();
    }
    CloseConnection(db);
    if(query.lastError().isValid())
    {
        qDebug() << "Sql error:" << query.lastError();
        return false;
    }

    return true;

}

QSqlQuery CSQLDatabase::ExecuteQuery(QString qsSql)
{
    QSqlDatabase db = OpenConnection();
    if(!db.isOpen())
        return QSqlQuery();

    QSqlQuery query = db.exec(qsSql);
    //CloseConnection(db);

    if(query.lastError().isValid())
    {
        qDebug() << "Sql error:" << query.lastError();
        return QSqlQuery();
    }

    return query;
}



#include "connectdb.h"

ConnectDB::ConnectDB(QObject *parent) : QObject(parent)
{
    qDebug() << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost"); // 10.12.240.91
    db.setDatabaseName("idea-craft.net");
    db.setUserName("root");
    db.setPassword("1234");
    open();
}

QSqlQuery ConnectDB::getQuery()
{
    return QSqlQuery(db);
}

QSqlQuery ConnectDB::exec(QString query)
{
    QSqlQuery qr = db.exec(query);
    if (qr.lastError().type()==QSqlError::NoError) {
        return qr;
    }
    else {
        qDebug() << qr.lastError().text() << qr.lastError().type();
        db.close();
        db.open();
        return exec(query);
    }
}

void ConnectDB::open()
{
    if (!db.open()) {
        qDebug() << "Cannot open database" << db.lastError().text();
        //dbOpenTimer.callOnTimeout(SLOT(ConnectDB::open()));
        connect(&dbOpenTimer, &QTimer::timeout, this, &ConnectDB::open);
        dbOpenTimer.start(1000);
        return;
    }else {
        qDebug() << "Connect to DB sucsess";
    }
}

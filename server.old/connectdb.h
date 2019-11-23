#ifndef CONNECTDB_H
#define CONNECTDB_H

#include <QtCore>
#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class ConnectDB : public QObject
{
    Q_OBJECT
public:
    explicit ConnectDB(QObject *parent = nullptr);
    QSqlQuery getQuery();
    QSqlQuery exec(QString query);

signals:
    void dbConected();

public slots:
    void open();
private:
    QSqlDatabase db;
    QTimer dbOpenTimer;
};

#endif // CONNECTDB_H

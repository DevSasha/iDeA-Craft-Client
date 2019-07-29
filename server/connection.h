#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDataStream>
#include "connectdb.h"

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(QTcpSocket *socket, ConnectDB *db, QObject *parent = nullptr);
    ~Connection();
    int write(QByteArray msg);

signals:

public slots:
    void readyRead();
    void disconnect();

private:
    bool clientAuthorized = false;
    QTcpSocket *socket;
    QByteArray message;
    ConnectDB *db;

    int checkUpdate(QJsonObject *obj);
    int logIn(QJsonObject *obj);
    int sigIn(QJsonObject *obj);
};

#endif // CONNECTION_H

#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include <QtCore>
#include <QObject>
#include <QString>
#include <QVector>
#include "connectdb.h"
#include "connection.h"

QT_BEGIN_NAMESPACE
//class QLabel;
class QTcpServer;
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

//! [0]
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server();

public slots:
    void broadcast(QString msg);

private slots:
    void sessionOpened();
    void newConnection();

private:
    QTcpServer *tcpServer = nullptr;
    QList<Connection *> connections;
    QNetworkSession *networkSession = nullptr;
    ConnectDB *db;
    QTimer dbTimer;
};
//! [0]

#endif

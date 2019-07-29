#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QSettings>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client();
    ~Client();
    int send(QByteArray msg);

signals:
    void ReadyRead(QByteArray msg);

private slots:
    void read();
    void Connect();
    void displayError(QAbstractSocket::SocketError socketError);
    void sessionOpened();

private:
    QSettings *cfg;
    QString host;
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QList<QByteArray> messages;

    QNetworkSession *networkSession = nullptr;
};

#endif

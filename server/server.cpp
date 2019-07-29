#include "server.h"

Server::Server()
{
    db = new ConnectDB();
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Server::sessionOpened);

        networkSession->open();
    } else {
        sessionOpened();
    }
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);
}

void Server::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    tcpServer = new QTcpServer();
    if (!tcpServer->listen(QHostAddress::Any, 22865)) {
        qDebug() << tr("Fortune Server")
                 << tr("Unable to start the server: %1.")
                    .arg(tcpServer->errorString());
        return;
    }else
        qDebug() << "The server is running on IP:" << tcpServer->serverAddress() << "port:" << tcpServer->serverPort();
}

void Server::newConnection()
{
    connections.push_back(new Connection(tcpServer->nextPendingConnection(), db));
}

void Server::broadcast(QString msg)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);

    out << msg; //fortunes[QRandomGenerator::global()->bounded(fortunes.size())];
    //tcpSockets.last()->write(block);
}

/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QMessageBox>
#include <QtNetwork>
#include "client.h"

Client::Client() : tcpSocket(new QTcpSocket(this))
{
    //QHostAddress address(hostAddres);
    //QList<QHostAddress> ipAddressesList;
    // find out name of this machine
    //QString name = QHostInfo::localHostName();
    //qDebug() << name;
    //if (!name.isEmpty()) ipAddressesList.push_back(QHostAddress(name));
    //if (name != QLatin1String("localhost"))
    // find out IP addresses of this machine
    //ipAddressesList << QNetworkInterface::allAddresses();
    //ipAddressesList << QHostInfo::addresses();
    // add non-localhost addresses
    //for (int i = 0; i < ipAddressesList.size(); ++i)
        //if (!ipAddressesList.at(i).isLoopback())
            //qDebug() << ipAddressesList.at(i).toString();

    in.setDevice(tcpSocket);
    //in.setVersion(QDataStream::Qt_5_10);

    connect(tcpSocket, &QIODevice::readyRead, this, &Client::read);
    //connect(tcpSocket, &QTcpSocket::disconnected, this, &Client::Disconnect);
    //connect(tcpSocket, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &Client::displayError);

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
        connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);
        networkSession->open();
    }
    Connect();
}

Client::~Client()
{
    delete tcpSocket;
    delete networkSession;
}

int Client::send(QByteArray msg)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);

    out << msg;

    tcpSocket->write(msg);
    return 0;
}

void Client::read()
{
    QByteArray msg = tcpSocket->readAll();
    if(!msg.size()) messages.push_back(msg);
    emit ReadyRead(msg);
}

void Client::Connect()
{
    tcpSocket->connectToHost("idea-craft.space", 22865);
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        QMessageBox::critical(nullptr, "Ошибка", "Потеряно соединение с сервером");
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(nullptr, "Ошибка",
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        Connect();
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(nullptr, "Ошибка", "Соединение было отклонено узлом. Убедитесь, что сервер работает"
                                    " и, что имя хоста и порт верны.");
        Connect();
        break;
    default:
        QMessageBox::information(nullptr, tr("Fortune Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }
}

void Client::sessionOpened()
{
    // Save the used configuration
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

    //statusLabel->setText(tr("This examples requires that you run the "
    //"Fortune Server example as well."));
}

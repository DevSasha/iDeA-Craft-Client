#include "connection.h"

Connection::Connection(QTcpSocket *socket, ConnectDB *db,QObject *parent) : QObject(parent)
{
    qDebug() << "new connection";
    this->socket = socket;
    this->db = db;
    connect(socket, &QTcpSocket::readyRead, this, &Connection::readyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Connection::disconnect);
}

Connection::~Connection(){
    disconnect();
}

//QByteArray Connection::read()
//{
//    if(!message.isEmpty()){
//        auto msg = message;
//        message.clear();
//        qDebug() << message;
//        return msg;
//    }
//    return 0;
//}

int Connection::write(QByteArray msg)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_5_10);

    out << msg;

    socket->write(msg);
    return 0;
}

void Connection::readyRead()
{
    QByteArray msg = socket->readAll();
    QJsonDocument doc = QJsonDocument::fromBinaryData(msg);
    QJsonObject root = doc.object();

    QJsonValue method = root.value("method");
    if(method.isString()){
        QString strMethod = method.toString();
        qDebug() << strMethod;
        if(strMethod == "authorization"){
            logIn(&root);
        }
        else if(strMethod == "registration"){
            sigIn(&root);
        }
        else if(strMethod == "chekUpdate"){

        }

    }else{
        qDebug() << "Error, uncorectly reqest";
    }

    //QDataStream in;
    //in.startTransaction();

    //QByteArray input;
    //in >> input;
    //if (!in.commitTransaction()) return;
}

void Connection::disconnect()
{
    socket->disconnect();
    socket->deleteLater();
    qDebug() << "Disonnect";
}

int Connection::checkUpdate(QJsonObject *obj)
{
    return 0;
}

int Connection::logIn(QJsonObject *obj)
{
    QString login = obj->value("login").toString();
    QString password = obj->value("password").toString();

    QSqlQuery qr = db->exec("select * from `users`;");
    QJsonObject root;
    root.insert("method", "authorization");
    root.insert("version", "1");
    while(qr.next()){
        if(qr.value(2).toString() == login){
            if(qr.value(3).toString() == password){
                clientAuthorized = true;
                root.insert("response", "sucsess");
                root.insert("nikname", qr.value(1).toString());
                break;
            }else root.insert("response", "wrong password");
        }
    }
    if(root.value("response").toString().size() == 0) root.insert("response", "wrong login");
    QJsonDocument doc(root);
    write(doc.toBinaryData());
    return -1;
}

int Connection::sigIn(QJsonObject *obj)
{
    QString nikname = obj->value("nikname").toString();
    QString login = obj->value("login").toString();
    QString password = obj->value("password").toString();
    QString soult = "123";//root.value("soult").toString();

    QSqlQuery qr = db->exec("insert into `users` (`nikname`, `eMail`, `password`, `soult`) "
                            "values ('" + nikname + "', '" + login + "', '" + password + "', '" + soult +"')");
    QJsonObject root;
    root.insert("method", "registration");
    root.insert("version", "1");
    root.insert("response", "sucsess");
    if(!qr.exec()){
        root.insert("response", qr.lastError().text());
    }else{
        root.insert("response", "sucsess");
    }
    QJsonDocument doc(root);
    write(doc.toBinaryData());
    return 0;
}

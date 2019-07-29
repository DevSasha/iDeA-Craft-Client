#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include <QFile>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server s;

//    QDir Roaming(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
//    if(!Roaming.exists()) Roaming.mkdir(Roaming.path());

//    QFile config(Roaming.path() + "\\instanse1.json");
//    if(config.open(QFile::ReadWrite)){
//        if(config.readAll().size() > 10){
//            QJsonDocument doc = QJsonDocument::fromJson(config.readAll());
//            QJsonObject root = doc.object();
//        }else{

//        }

//        if(root.value("login").isString()){

//        }
//        qDebug() << "Close";
//        config.close();
//    }

    return a.exec();
}

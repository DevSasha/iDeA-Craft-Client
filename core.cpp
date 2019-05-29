#include "core.h"

Core::Core(QApplication *parent) : QObject(parent)
{
    app = parent;
    manager = new QNetworkAccessManager;
    //load();
    qDebug() << "Checking update...";
    checkUpdate();
}

void Core::checkUpdate()
{
    connect(manager, &QNetworkAccessManager::finished, this, &Core::takeUpdeteNews);
    QNetworkRequest request;    // Отправляемый запрос
    request.setUrl(QString("http://idea-craft.space/repo/idea-launcher/versions.json")); // Устанавлвиваем URL в запрос
    manager->get(request);      // Выполняем запрос
}

void Core::authorized()
{
    window_auth->close();
    window_main->setNik(window_auth->nik);
    window_main = new MainWindow;
    connect(inst, &Instanse::processUpdate, window_main, &MainWindow::processUpdate);
    window_main->show();
}

void Core::takeUpdeteNews(QNetworkReply *reply)
{
    if(reply->error()){
            qDebug() << "ERROR1" << reply->errorString();
            QMessageBox::critical(nullptr, "Error update", "Can`t get list versions");
            QApplication::exit();
    } else {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = doc.object();
        QJsonValue lastVersion = root.value("lastVersion");
        QString lastVer;
        if(lastVersion.isString()) lastVer = lastVersion.toString(); else {
            qDebug() << lastVersion.type();
            QMessageBox::critical(nullptr, "Error update", "Can`t read last version");
            QApplication::exit();
        }

        if(lastVer == appVersion){
            disconnect(manager, &QNetworkAccessManager::finished, this, &Core::takeUpdeteNews);
            load();
        }else {
            disconnect(manager, &QNetworkAccessManager::finished, this, &Core::takeUpdeteNews);
            //connect(manager, &QNetworkAccessManager::finished, this, &Core::takeUpdate);
            //QNetworkRequest request;    // Отправляемый запрос
            //request.setUrl(QString("http://idea-craft.space/repo/idea-launcher/iDeA-Craft-Updater.exe")); // Устанавлвиваем URL в запрос
            //manager->get(request);      // Выполняем запрос
        }
    }
}

void Core::takeUpdate(QNetworkReply* reply) // Only for Windows
{
    if(reply->error()){
        qCritical() << "Can`t download updater" << reply->errorString();
    } else {
        QFile file("./Updater.exe");
        if(file.open(QFile::WriteOnly)){
            file.write(reply->readAll());  // ... и записываем всю информацию со страницы в файл
            file.close();                  // закрываем файл
        }

        QProcess updater;
        updater.setWorkingDirectory(".");
        QList<QString> args;
        args << "-Ver" << appVersion;
        updater.startDetached("Updater", args);
        QApplication::exit();
    }
}

void Core::load()
{
    qDebug() << "loading...";
    //cfgs = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/configs.ini", QSettings::IniFormat);
    cfgs = new QSettings("DrSasha", "iDeA-Craft Launcher");
    client = new Client;
    inst = new Instanse("lite12");
    window_auth = new Authorization(client);
        connect(window_auth, &Authorization::authorized, this, &Core::authorized);
        //connect(window_main, &MainWindow::startInst, this, &Core::startInst);
    Local = new QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
        if(!Local->exists())Local->mkdir(Local->path());

    cfgs->beginGroup("AuthorizationData");
    if(!cfgs->value("login").isNull() && !cfgs->value("password").isNull()){
        window_auth->login = cfgs->value("login").toString();
        window_auth->password = cfgs->value("password").toString();
        window_auth->auth();
    }else window_auth->show();
    cfgs->endGroup();
    //connect(inst, &Instanse::downloadComplete, this, &Core::downloadComplete);
}

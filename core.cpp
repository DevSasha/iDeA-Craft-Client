#include "core.h"

Core::Core(QApplication *parent) : QObject(parent)
{
    app = parent;
    manager = new QNetworkAccessManager;
    //load();
    checkUpdate();
    inst = new Instanse("lite");
}

void Core::checkUpdate()
{
    connect(manager, &QNetworkAccessManager::finished, this, &Core::takeUpdeteNews);
    QNetworkRequest request;    // Отправляемый запрос
    request.setUrl(QString("http://drsaha.hopto.org/repo/idea-launcher/versions.json")); // Устанавлвиваем URL в запрос
    manager->get(request);      // Выполняем запрос
}

void Core::authorized()
{
    cfgs->beginGroup("AuthorizationData");
    cfgs->setValue("login", window_auth->login);
    cfgs->setValue("password", window_auth->password);
    cfgs->endGroup();
    window_auth->close();
    window_main->setNik(window_auth->nik);
    window_main->show();
}

void Core::startInst(QString name)
{
    inst->run(window_auth->nik);
}

void Core::downloadInst(QString name)
{
    inst->download();
    connect(inst, &Instanse::processUpdate, window_main, &MainWindow::processUpdate);
    connect(inst, &Instanse::downloadComplete, this, &Core::downloadComplete);
}

void Core::downloadComplete()
{
    window_main->installationComplete();
    qDebug() << "Installation complete";
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
            connect(manager, &QNetworkAccessManager::finished, this, &Core::takeUpdate);
            QNetworkRequest request;    // Отправляемый запрос
            request.setUrl(QString("http://drsaha.hopto.org/repo/idea-launcher/iDeA-Craft-Updater.exe")); // Устанавлвиваем URL в запрос
            manager->get(request);      // Выполняем запрос
        }
    }
}

void Core::takeUpdate(QNetworkReply* reply)
{
    if(reply->error()){
            qDebug() << "ERROR"  << reply->errorString();
            QMessageBox::critical(nullptr, "Error update", "Can`t download updater");
            QApplication::exit();
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
    //cfgs = new QSettings(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/configs.ini", QSettings::IniFormat);
    cfgs = new QSettings("DrSasha", "iDeA-Craft Launcher");
    client = new Client;
    window_auth = new Authorization(client);
    window_main = new MainWindow;
        connect(window_auth, &Authorization::authorized, this, &Core::authorized);
        connect(window_main, &MainWindow::startInst, this, &Core::startInst);
        connect(window_main, &MainWindow::downloadInst, this, &Core::downloadInst);
    Local = new QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
        if(!Local->exists())Local->mkdir(Local->path());

    cfgs->beginGroup("AuthorizationData");
    if(!cfgs->value("login").isNull() && !cfgs->value("password").isNull()){
        window_auth->login = cfgs->value("login").toString();
        window_auth->password = cfgs->value("password").toString();
        window_auth->auth();
    }else window_auth->show();
    cfgs->endGroup();
}

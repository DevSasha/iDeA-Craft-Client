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
    request.setUrl(QString("http://launcher.idea-craft.space/versions.json")); // Устанавлвиваем URL в запрос
    manager->get(request);      // Выполняем запрос
}

void Core::authorized()
{
    qDebug() << "Creating main window";
    window_main = new MainWindow;
    window_main->setNik(window_auth->nik);
    //connect(inst, &Instanse::processUpdate, window_main, &MainWindow::processUpdate);
    window_auth->hide();
    window_main->show();
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

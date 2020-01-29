#include "core.h"

Core::Core() {
    manager = new QNetworkAccessManager;
#ifndef DEVELOP_MODE
	Update *upd = new Update();
	connect(upd, &Update::updated, this, &Core::load);
	upd->checkUpdate();
#else
	load();
#endif
}

void Core::authorized() {
    qDebug() << "Creating main window";
	window_main = new MainWindow();
	window_main->setNickname(window_auth->login);
    window_auth->hide();
    window_main->show();
}

void Core::load() {
    qDebug() << "loading...";
	window_auth = new Authorization();
        connect(window_auth, &Authorization::authorized, this, &Core::authorized);
    Local = new QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
        if(!Local->exists())Local->mkdir(Local->path());

	window_auth->show();
}

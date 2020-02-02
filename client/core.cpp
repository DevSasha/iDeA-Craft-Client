#include "core.h"

Core::Core() {
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
	window_auth->deleteLater();
    window_main->show();
}

void Core::load() {
    qDebug() << "loading...";
	window_auth = new Authorization();
        connect(window_auth, &Authorization::authorized, this, &Core::authorized);
}

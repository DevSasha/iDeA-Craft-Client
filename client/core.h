#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QSettings>
#include <QProcess>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QInputDialog>
#include "authorization.h"
#include "mainwindow.h"
#include "backend/instanse.h"
#include "backend/update.h"

class Core : public QObject {
    Q_OBJECT
public:
    explicit Core();
    const QString appVersion = "0.0.2.10";
signals:

public slots:
	void authorized();
	void load();

private:
    QSettings *cfgs;
	//Client *client;
    Authorization *window_auth;
    MainWindow *window_main;
    QProcess *runMC;
    QDir *Local, *Temp;
    Instanse *inst;
    QNetworkAccessManager *manager;
};

#endif // CORE_H

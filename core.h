#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QSettings>
#include <QProcess>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QInputDialog>
#include <QHostAddress>
#include "authorization.h"
#include "client.h"
#include "mainwindow.h"
#include "instanse.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);
    int checkUpdate();

    const QString appVersion = "0.1";
signals:

public slots:
    void authorized();
    void startInst(QString name);
    void downloadInst(QString name);
    void downloadComplete();
    void takeUpdeteNews(QByteArray msg);
    void takeUpdate(QNetworkReply* reply);

private:
    void load();

    QSettings *cfgs;
    Client *client;
    Authorization *window_auth;
    MainWindow *window_main;
    QProcess *runMC;
    QDir *Local, *Temp;
    Instanse *inst;
};

#endif // CORE_H

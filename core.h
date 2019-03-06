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
#include <QTimer>
#include <QTextStream>
#include "authorization.h"
#include "client.h"
#include "mainwindow.h"
#include "instanse.h"

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QApplication *parent = nullptr);
    //void checkUpdate();

    const QString appVersion = "0.1";
signals:

public slots:
    void checkUpdate();
    void authorized();
    void startInst(QString name);
    void downloadInst(QString name);
    void downloadComplete();
    void takeUpdeteNews(QNetworkReply* reply);
    void takeUpdate(QNetworkReply* reply);

private:
    void load();
    void chekedUpdate();

    QSettings *cfgs;
    Client *client;
    Authorization *window_auth;
    MainWindow *window_main;
    QProcess *runMC;
    QDir *Local, *Temp;
    Instanse *inst;
    QNetworkAccessManager *manager;
    QApplication *app;
};

#endif // CORE_H

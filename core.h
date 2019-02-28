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
signals:

public slots:
    void authorized();
    void startInst(QString name);
    void downloadInst(QString name);
    void downloadComplete();

private:
    QApplication *app;
    QSettings *cfgs;
    Client *client;
    Authorization *window_auth;
    MainWindow *window_main;
    QProcess *runMC;
    QDir *Local;
    Instanse *inst;
};

#endif // CORE_H

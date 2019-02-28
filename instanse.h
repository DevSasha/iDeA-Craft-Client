#ifndef INSTANSE_H
#define INSTANSE_H

#include <QObject>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QDir>
#include <QStandardPaths>
#include <QFile>

class Instanse : public QObject
{
    Q_OBJECT
public:
    explicit Instanse(QString nameInst, QObject *parent = nullptr);
    ~Instanse();

signals:
    void processUpdate(int status);
    void downloadComplete();

public slots:
    void takeFile(QNetworkReply *reply);
    void getFile();
    void takeMetadata(QNetworkReply *reply);

private:
    bool checkInstallation(QString mode);

    bool installed = false;
    QDir *instanses, *instDir;
    QString instName, urlRepo; // = "http://idea-craft.net/repository/mc/";
    QNetworkAccessManager *manager;
    QList<QString> filenames, cheksums, filetypes;
    QList<int> filesizes;

    QString curDownloadingName, curDownloadingSum;
    int curDownloadingSize, allFile, downloadedFiles = 0;
};

#endif // INSTANSE_H

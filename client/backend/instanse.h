#ifndef INSTANSE_H
#define INSTANSE_H

#include <QObject>
#include <QProcess>
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
	explicit Instanse(uint instanseId);
    ~Instanse();
    void download();
    void run(QString nik);

signals:
    void infoUpdated();

    void processUpdate(int status);
    void downloadComplete();

private slots:
    void takeMetadata(QNetworkReply *reply);
    void takeSHA256(QNetworkReply *reply);
    void startDownload();
    void startRun();
    void takeFile(QNetworkReply *reply);

private:
	QDir *instanse;
	QString instanseName, instanseVersion, instanseMC, instanseForge, nikname;

    bool checkInstallation(QString mode);
    void wget(QUrl url, const QMetaMethod &method);
    void getFile();

    bool installed = false, infoUpdate = false;


    QString urlRepo; // = "http://idea-craft.net/repository/mc/";
    QList<QString> filenames, cheksums, filetypes;
    QList<int> filesizes;

    QString curDownloadingName, curDownloadingSum;
    int curDownloadingSize, allFile, downloadedFiles = 0;
};

#endif // INSTANSE_H

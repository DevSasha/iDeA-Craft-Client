#ifndef DOWNLOADWORKER_H
#define DOWNLOADWORKER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DownloadWorker : public QObject {
	Q_OBJECT
public:
	DownloadWorker();
	~DownloadWorker();
	void setFileList(const QList<QString> names, const QList<QString> paths, const QList<QString> hashes);
	void start();

signals:
	void onDownloaded();

private slots:
	void saveFile(QNetworkReply *reply);
	void downloadFile();
private:
	QList<QString> names {}, paths {}, hashes {};
	QNetworkAccessManager *http;
	quint64 currentFile = 0;
};

#endif // DOWNLOADWORKER_H

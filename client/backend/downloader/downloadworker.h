#ifndef DOWNLOADWORKER_H
#define DOWNLOADWORKER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QNetworkAccessManager>
#include <QCryptographicHash>
#include <backend/downloader/downloadfile.h>

class DownloadWorker : public QObject {
	Q_OBJECT
public:
	DownloadWorker();
	~DownloadWorker();
	void setFileList(QList<DownloadFile *> files);
	void start();

signals:
	void onDownloaded(int);
	void onFinish(quint64);
	void onError(QString);

public slots:
	void fileDownloaded();
	void nextFile();
	void error(QString msg);

private:
	QList<DownloadFile *> files;
	QNetworkAccessManager *http;
	DownloadFile *current = nullptr;
	quint64 countDownloadedFiles = 0;
	quint64 needDownload = 0;

	void downloadFile();
};

#endif // DOWNLOADWORKER_H

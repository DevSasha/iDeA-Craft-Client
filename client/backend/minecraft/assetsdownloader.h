#ifndef ASSETSDOWNLOADER_H
#define ASSETSDOWNLOADER_H

#include <QObject>
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QCryptographicHash>
#include <backend/httprequest.h>
#include <backend/downloader/downloadworker.h>

class AssetsDownloader : public QObject {
	Q_OBJECT
public:
	explicit AssetsDownloader(QDir mc, QJsonObject assetIndex);
	~AssetsDownloader();

	void update();

signals:
	void updated();
	void onProgressUpdate(int);
	void metaUpdated();

public slots:
	void startDownload();
	void replyAssetMeta(QNetworkReply * reply);
	void updateProgress(int progress);

private:
	QDir assets;
	QFile index;
	DownloadWorker *dw;

	QString version;
	QString sha1;
	QString url;

	QList<DownloadFile *> files;

	void downloadAssetIndex();
	void parseAssetIndex(QByteArray &data);
};

#endif // ASSETSDOWNLOADER_H

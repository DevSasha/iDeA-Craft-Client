#ifndef ASSETSDOWNLOADER_H
#define ASSETSDOWNLOADER_H

#include <QObject>
#include <QDir>
#include <QStandardPaths>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCryptographicHash>
#include <backend/minecraft/minecraftversion.h>
#include <backend/downloader/downloadworker.h>

class AssetsDownloader : public QObject {
	Q_OBJECT
public:
	explicit AssetsDownloader(MinecraftVersion *version);

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
	QNetworkAccessManager mng;
	DownloadWorker *dw;

	QString version;
	QString sha1;
	QString url;

	QList<DownloadFile *> files;

	void downloadAssetIndex();
	void parseAssetIndex(QByteArray &data);
};

#endif // ASSETSDOWNLOADER_H

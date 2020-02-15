#ifndef MINECRAFTVERSION_H
#define MINECRAFTVERSION_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <backend/minecraft/assetsdownloader.h>

class MinecraftVersion : public QObject {
	Q_OBJECT
public:
	explicit MinecraftVersion(QString version);
	~MinecraftVersion();

	void update();
	void download();
	QString getVersion() const;
	QString getMainClass() const;
	QString getMinecraftArguments() const;

signals:
	void updated();
	void downloadProgress(QString, int);

public slots:
	void replyVersionMeta(QNetworkReply *reply);
	void progressChanged(int progress);

private:
	static QJsonArray versions;
	bool isUpdated = false;
	QString version;
	QNetworkAccessManager mng;
	QDir dir;

	QJsonObject assetIndex;
	QJsonArray libraries;
	QString mainClass;
	QString minecraftArguments;

	AssetsDownloader *assets;
};

#endif // MINECRAFTVERSION_H

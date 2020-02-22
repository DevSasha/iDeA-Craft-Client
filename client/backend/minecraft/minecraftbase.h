#ifndef MINECRAFTVERSION_H
#define MINECRAFTVERSION_H

#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <backend/httprequest.h>
#include <backend/minecraft/assetsdownloader.h>
#include <backend/minecraft/libraries.h>

class MinecraftBase : public QObject {
	Q_OBJECT
public:
	explicit MinecraftBase(QString version);
	~MinecraftBase();

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
	QDir dir;

	QJsonObject assetIndex;
	QJsonArray libraries;
	QString mainClass;
	QString minecraftArguments;

	Libraries *libs;
	AssetsDownloader *assets;
};

#endif // MINECRAFTVERSION_H

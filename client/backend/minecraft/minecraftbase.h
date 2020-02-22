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

	virtual void update();
	virtual void download();
	QString getVersion() const;
	QString getMainClass() const;
	QString getMinecraftArguments() const;

signals:
	void updated();
	void downloadProgress(QString, int);

public slots:
	void replyVersionMeta(QNetworkReply *reply);
	void progressChanged(int progress);

	void assetDownloadStart();
	virtual void assetDownloadFinish();
	void librariesDownloadStart();
	virtual void librariesDownloadFinish();

protected slots:
	virtual void metaParsed();

protected:
	static QJsonArray versions;
	bool isUpdated = false;
	QString version;
	QString downloadingMessage = "";
	QDir dir;

	QJsonObject assetIndex;
	QJsonArray libraries;
	QString mainClass;
	QString minecraftArguments;

	Libraries *libs;
	AssetsDownloader *assets;
};

#endif // MINECRAFTVERSION_H

#ifndef MINECRAFTVERSION_H
#define MINECRAFTVERSION_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class MinecraftVersion : public QObject {
	Q_OBJECT
public:
	explicit MinecraftVersion(QString version);
	~MinecraftVersion();

	bool isUpdated() const;
	QJsonObject getAssetIndex() const;
	QJsonArray getLibraries() const;
	QString getMainClass() const;
	QString getMinecraftArguments() const;

signals:

public slots:
	void replyVersionMeta(QNetworkReply *reply);

private:
	static QJsonArray versions;
	bool updated = false;
	QString version;
	QNetworkAccessManager mng;

	QJsonObject assetIndex;
	QJsonArray libraries;
	QString mainClass;
	QString minecraftArguments;
};

#endif // MINECRAFTVERSION_H

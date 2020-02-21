#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <defines.h>
#include <backend/httprequest.h>

class APIRequest : public HTTPRequest {
	Q_OBJECT
public:
	explicit APIRequest(QString mathod);
	static void addDefaultQueryItem(QString key, QString value);

signals:
	void finished(QJsonObject *);

public slots:
	void reply(QNetworkReply *reply);

private:
	static std::vector<QString> defaultKeys, defaultValues;
};

#endif // APIREQUEST_H

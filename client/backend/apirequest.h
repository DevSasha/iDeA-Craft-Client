#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <defines.h>

class APIRequest : public QObject {
	Q_OBJECT
public:
	explicit APIRequest(QString mathod);
	~APIRequest();
	void addQueryItem(QString key, QString value);
	void addDefaultQueryItem(QString key, QString value);
	void send();

signals:
	void finished(QJsonObject *);

public slots:
	void reply(QNetworkReply *reply);

private:
	static std::vector<QString> defaultKeys, defaultValues;

	QNetworkAccessManager *mng = nullptr;
	QUrlQuery form;
	QString method;
};

#endif // APIREQUEST_H

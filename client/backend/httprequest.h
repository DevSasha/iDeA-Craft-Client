#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <queue>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>

class HTTPRequest : public QObject {
	Q_OBJECT
public:
	explicit HTTPRequest(QString url, QString method = "GET");
	~HTTPRequest();

	void addQueryItem(QString key, QString value);
	void send();

signals:
	void finished(QNetworkReply *);

public slots:
	void reply(QNetworkReply *reply);

protected:
	void setup();

	QString url;
	QString method;
	QUrlQuery form;
	QNetworkRequest request;
	QByteArray postData;

private:
	static QNetworkAccessManager *mng;
	static std::queue<HTTPRequest *> query;

	void _send();
	void close();
};

#endif // HTTPREQUEST_H

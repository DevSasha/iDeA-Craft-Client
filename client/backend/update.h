#ifndef UPDATE_H
#define UPDATE_H

#include "defines.h"
#include <QObject>
#include <QtDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

class Update : public QObject
{
	Q_OBJECT
public:
	explicit Update(QObject *parent = nullptr);

signals:

public slots:
	void checkUpdate();
	void onResponse(QNetworkReply *reply);
	void parseBody(QJsonObject body);

private:
	QNetworkAccessManager *manager;
};

#endif // UPDATE_H

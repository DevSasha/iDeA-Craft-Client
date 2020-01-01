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
#include <QStandardPaths>
#include <backend/downloader/downloadworker.h>
#include <QApplication>
#include <QProcess>

class Update : public QObject
{
	Q_OBJECT
public:
	explicit Update(QObject *parent = nullptr);

signals:
	void updated();

public slots:
	void checkUpdate();
	void onResponse(QNetworkReply *reply);
	void parseBody(QJsonObject body);
	void startDownload(const QList<DownloadFile *> &files);

	void downloadFinished(quint64 countDownloadedFiles);
	void finishUpdate();
	void restart();
	void error(QString msg);

private:
	QNetworkAccessManager *manager;
	DownloadWorker *dw;
};

#endif // UPDATE_H

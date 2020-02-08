#ifndef UPDATE_H
#define UPDATE_H

#include "defines.h"
#include <QObject>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QStandardPaths>
#include <backend/downloader/downloadworker.h>
#include <QApplication>
#include <QProcess>
#include <backend/config.h>
#include <backend/apirequest.h>

class Update : public QObject
{
	Q_OBJECT
public:
	explicit Update(QObject *parent = nullptr);

signals:
	void updated();

public slots:
	void checkUpdate();
	void parseBody(QJsonObject *body);
	void startDownload(const QList<DownloadFile *> &files);

	void downloadFinished(quint64 countDownloadedFiles);
	void finishUpdate();
	void restart();
	void error(QString msg);

private:
	DownloadWorker *dw;
	QString branch;
    QString platform;
	QString mainFile;
};

#endif // UPDATE_H

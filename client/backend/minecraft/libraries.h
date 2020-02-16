#ifndef LIBRARIES_H
#define LIBRARIES_H

#include <QObject>
#include <QDebug>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <backend/downloader/downloadworker.h>

class Libraries : public QObject {
	Q_OBJECT
public:
	explicit Libraries(QDir mc, QJsonArray libraries, QObject *parent = nullptr);

signals:
	void downloaded();
	void updateProgress(int);

public slots:
	void download();

private:
	QDir libraries;
	QJsonArray libs;
	DownloadWorker *dw;
};

#endif // LIBRARIES_H

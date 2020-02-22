#ifndef LIBRARIES_H
#define LIBRARIES_H

#include <QObject>
#include <QDebug>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVersionNumber>
#include <backend/downloader/downloadworker.h>

class Libraries : public QObject {
	Q_OBJECT
public:
	explicit Libraries(QDir mc, QJsonArray libraries, QObject *parent = nullptr);
	Libraries(QDir mc);
	~Libraries();
	void addLibrary(QJsonObject lib);
	QString getClassPath();
	void addClassPath(QString path);

signals:
	void downloaded();
	void updateProgress(int);

public slots:
	void download();
	void onUpdateProgress(int progress);

private:
	QDir libsDir;
	QString os = "", arch = "";
	std::list<QJsonObject> libraries;
	std::list<QString> classPaths;
	DownloadWorker *dw;
};

#endif // LIBRARIES_H

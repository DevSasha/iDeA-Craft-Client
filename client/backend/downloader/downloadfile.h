#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <QObject>
#include <QCryptographicHash>
#include <QFile>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDataStream>

class DownloadFile : public QObject {
	Q_OBJECT
public:
	DownloadFile(QString name, QString hash, QString path, QString uri, QCryptographicHash::Algorithm alg);
	~DownloadFile();
	bool isCorrect();

signals:
	void onDownload();
	void onError(QString);

public slots:
	void get(QNetworkAccessManager *http);
	void take(QNetworkReply *reply);
	void save();
	bool check();

private:
	QString name, hash, path, uri;
	QCryptographicHash::Algorithm alg;
	QByteArray data;
	QFile *file = nullptr;
	bool correct;
};

#endif // DOWNLOADFILE_H

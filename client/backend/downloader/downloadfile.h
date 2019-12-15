#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <QObject>
#include <QCryptographicHash>
#include <QFile>
#include <QString>
#include <QNetworkAccessManager>

class DownloadFile : public QObject {
	Q_OBJECT
public:
	DownloadFile(QString name, QString hash, QString path, QString uri, QCryptographicHash::Algorithm alg);
	~DownloadFile();
	bool isCorrect();

public slots:
	void get();
	void save();
	bool checkData();

private:
	QString name, hash, path, uri;
	QCryptographicHash::Algorithm alg;
	QByteArray data;
	QFile *file = nullptr;
	QNetworkAccessManager *http;
	bool correct;
};

#endif // DOWNLOADFILE_H

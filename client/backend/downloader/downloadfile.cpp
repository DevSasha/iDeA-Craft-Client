#include "downloadfile.h"

DownloadFile::DownloadFile(QString name, QString hash, QString path, QString uri, QCryptographicHash::Algorithm alg) {
	this->name = name;
	this->hash = hash;
	this->path = path;
	this->uri = uri;
	this->alg = alg;

	this->file = new QFile(this->path);
	if (!this->file->exists())
		this->correct = false;
	else {
		file->open(QIODevice::ReadOnly);

		this->data = file->readAll();
		this->correct = this->check();

		file->close();
	}

	connect(this, &DownloadFile::onDownload, this, &DownloadFile::deleteLater);
	connect(this, &DownloadFile::onError, this, &DownloadFile::deleteLater);
}

DownloadFile::~DownloadFile() {
	if (this->file != nullptr) {
		delete this->file;
	}
}

bool DownloadFile::isCorrect() {
	return this->correct;
}

void DownloadFile::get(QNetworkAccessManager *http) {
	if (this->correct) {
		emit this->onDownload();
	}

	QNetworkRequest req(this->uri);
	http->get(req);
}

void DownloadFile::take(QNetworkReply *reply) {
	if (reply->error()) {
			qCritical() << "Download error: " << reply->errorString();
	} else {
		this->data = reply->readAll();
		this->correct = this->check();

		if (this->correct) {
			this->save();
		} else {
			emit this->onError("Incorrect response");
		}
	}
}

void DownloadFile::save() {
	this->file->open(QIODevice::WriteOnly);
	QDataStream out(this->file);

	out << this->data;
	file->close();
	emit this->onDownload();
}

bool DownloadFile::check() {
	QString hash = QCryptographicHash::hash(this->data, this->alg).toHex();
	return hash == this->hash;
}

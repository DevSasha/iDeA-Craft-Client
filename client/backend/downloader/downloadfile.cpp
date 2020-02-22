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
		emit this->onCorrect();
	}

	connect(this, &DownloadFile::onDownload, this, &DownloadFile::deleteLater);
	connect(this, &DownloadFile::onError, this, &DownloadFile::deleteLater);
	connect(this, &DownloadFile::onCorrect, this, &DownloadFile::deleteLater);
}

DownloadFile::~DownloadFile() {
	if (this->file != nullptr) {
		delete this->file;
	}
	this->data.clear();
}

bool DownloadFile::isCorrect() {
	return this->correct;
}

void DownloadFile::get(QNetworkAccessManager *http) {
	if (this->correct) {
		emit this->onCorrect();
		return;
	}

	QNetworkRequest req(this->uri);
	http->get(req);
}

void DownloadFile::take(QNetworkReply *reply) {
	if (reply->error()) {
			qCritical() << "Download error: " << reply->errorString();
	} else {
		this->data = reply->readAll();
		reply->deleteLater();
		this->correct = this->check();

		if (this->correct) {
			this->save();
		} else {
			emit this->onError("Incorrect downloaded file");
		}
	}
}

void DownloadFile::save() {
	QStringList dirPath = this->path.split("/");
	dirPath.removeLast();
	QDir dir;
	dir.mkpath(dirPath.join("/"));

	this->file->open(QIODevice::WriteOnly);
	this->file->write(this->data);
	this->file->close();
	emit this->onDownload();
	emit this->onCorrect();
}

bool DownloadFile::check() {
	QString hash = QCryptographicHash::hash(this->data, this->alg).toHex();
	return hash == this->hash;
}

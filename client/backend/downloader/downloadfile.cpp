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
		this->correct = this->checkData();
	}
}

DownloadFile::~DownloadFile() {
	if (this->file != nullptr) {
		file->close();
		delete this->file;
	}
}

bool DownloadFile::isCorrect() {
	return this->correct;
}

bool DownloadFile::checkData() {
	QString hash = QCryptographicHash::hash(this->data, this->alg).toHex();
	return hash == this->hash;
}

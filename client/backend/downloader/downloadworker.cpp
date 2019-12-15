#include "downloadworker.h"

DownloadWorker::DownloadWorker() {
	this->http = new QNetworkAccessManager();
	connect(this, &DownloadWorker::finished, this, &DownloadWorker::deleteLater);
}

DownloadWorker::~DownloadWorker() {
	delete this->http;
}

void DownloadWorker::setFileList(const QList<DownloadFile *> files) {
	this->files = files;
}

void DownloadWorker::start() {
	//this->current = this->files.first();
	//this->downloadFile();
	this->nextFile();
}

void DownloadWorker::downloadFile() {
	if (this->current->isCorrect())
		this->nextFile();
	else {
		connect(this->http, &QNetworkAccessManager::finished, this->current, &DownloadFile::take);
		connect(this->current, &DownloadFile::onDownload, this, &DownloadWorker::nextFile);
		this->current->get(this->http);
	}
}

void DownloadWorker::nextFile() {
	if (this->files.size() == 0) {
		emit this->finished();
	} else {
		this->current = this->files.first();
		this->files.pop_front();
		this->downloadFile();
	}
}

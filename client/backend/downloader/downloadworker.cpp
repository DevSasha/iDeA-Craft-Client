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
	this->current = this->files.first();
	this->downloadFile();
}

void DownloadWorker::downloadFile() {
	if (this->current->isCorrect())
		this->nextFile();

	connect(this->http, &QNetworkAccessManager::finished, this->current, &DownloadFile::take);
	connect(this->current, &DownloadFile::onDownload, this, &DownloadWorker::nextFile);
	this->current->get(this->http);
}

void DownloadWorker::nextFile() {
	if (this->files.size() <= 1) {
		emit this->finished();
		return;
	}
	this->files.pop_front();
	this->current = this->files.first();
	this->downloadFile();
	emit this->onDownloaded();
}

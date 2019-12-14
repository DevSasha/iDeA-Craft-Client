#include "downloadworker.h"

DownloadWorker::DownloadWorker() {
	this->http = new QNetworkAccessManager();
	connect(this->http, &QNetworkAccessManager::finished, this, &DownloadWorker::saveFile);
}

DownloadWorker::~DownloadWorker() {
	delete this->http;
}

void DownloadWorker::start() {
	this->downloadFile();
}

void DownloadWorker::saveFile(QNetworkReply *reply) {
	// TODO Save File after download
	++this->currentFile;
	emit this->onDownloaded();
}

void DownloadWorker::downloadFile() {
	// TODO Start download file
}

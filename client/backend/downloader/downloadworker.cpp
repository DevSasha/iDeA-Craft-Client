#include "downloadworker.h"

DownloadWorker::DownloadWorker() {
	this->http = new QNetworkAccessManager();
	connect(this, &DownloadWorker::onFinish, this, &DownloadWorker::deleteLater);
}

DownloadWorker::~DownloadWorker() {
	delete this->http;
}

void DownloadWorker::setFileList(const QList<DownloadFile *> files) {
	this->files = files;
}

void DownloadWorker::start() {
	for (int i = 0; i < this->files.size(); ++i) {
		if (!this->files[i]->isCorrect()) {
			this->needDownload++;
		}
	}

	this->nextFile();
}

void DownloadWorker::fileDownloaded() {
	this->countDownloadedFiles++;
	emit this->onDownloaded((this->countDownloadedFiles * 100) / this->needDownload);
}

void DownloadWorker::downloadFile() {
	if (this->current->isCorrect())
		this->nextFile();
	else {
		connect(this->http, &QNetworkAccessManager::finished, this->current, &DownloadFile::take);
		connect(this->current, &DownloadFile::onCorrect, this, &DownloadWorker::nextFile);
		connect(this->current, &DownloadFile::onError, this, &DownloadWorker::error);
		connect(this->current, &DownloadFile::onDownload, this, &DownloadWorker::fileDownloaded);
		this->current->get(this->http);
	}
}

void DownloadWorker::nextFile() {
	if (this->files.size() == 0) {
		emit this->onFinish(this->countDownloadedFiles);
	} else {
		this->current = this->files.first();
		this->files.pop_front();
		this->downloadFile();
	}
}

void DownloadWorker::error(QString msg) {
	emit this->onError(msg);
}

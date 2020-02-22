#include "assetsdownloader.h"

AssetsDownloader::AssetsDownloader(QDir mc, QJsonObject assetIndex) : QObject() {
	this->assets = mc;
	this->version = assetIndex.value("id").toString();

	QJsonValue vSha1 = assetIndex.value("sha1");
	if (!vSha1.isString()) {
		qCritical() << "Asset sha1 is incorrect: " << vSha1.type();
	}
	this->sha1 = vSha1.toString();

	QJsonValue vUrl = assetIndex.value("url");
	if (!vUrl.isString()) {
		qCritical() << "Asset url is incorrect: " << vUrl.type();
	}
	this->url = vUrl.toString();
}

AssetsDownloader::~AssetsDownloader() {

}

void AssetsDownloader::update() {
	if (!this->assets.exists("assets")) {
		qDebug() << "Assets dir not exist. Creating...";
		this->assets.mkdir("assets");
	}
	this->assets.cd("assets");

	if (!this->assets.exists("indexes")) {
		qDebug() << "Indexes dir not exist. Creating...";
		this->assets.mkdir("indexes");
	}
	this->assets.cd("indexes");

	index.setFileName(this->assets.filePath(this->version + ".json"));
	if (!index.exists()) {
		qDebug() << "Index file for " << this->version << " not found. Downloading...";
		this->downloadAssetIndex();
	} else {
		if (!this->index.open(QIODevice::ReadOnly)) {
			return;
		}
		qDebug() << "Using saved asset index";

		QByteArray data = this->index.readAll();
		this->index.close();

		QString hash = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();
		if (this->sha1 != hash) {
			qDebug() << "Asset index file corrupted. Redownloding...";
			this->downloadAssetIndex();
		} else {
			this->parseAssetIndex(data);
		}
	}
}

void AssetsDownloader::replyAssetMeta(QNetworkReply *reply) {
	//this->mng->deleteLater();
	if (reply->error()) {
		qCritical() << "Can not get asset meta" << reply->errorString();
	} else {
		QByteArray data = reply->readAll();
		reply->deleteLater();

		QString hash = QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();
		if (this->sha1 != hash) {
			qCritical() << "Asset meta file corrupted. Hash: " << hash;
		}

		if (!this->index.open(QIODevice::WriteOnly)) {
			qWarning() << "Failed save asset index cache";
			return;
		}
		this->index.write(data);
		this->index.close();
		qDebug() << "Asset index cache saved";

		this->parseAssetIndex(data);
	}
}

void AssetsDownloader::updateProgress(int progress) {
	emit this->onProgressUpdate(progress);
}

void AssetsDownloader::downloadAssetIndex() {
	HTTPRequest *req = new HTTPRequest(this->url);
	connect(req, &HTTPRequest::finished, this, &AssetsDownloader::replyAssetMeta);
	req->send();
}

void AssetsDownloader::parseAssetIndex(QByteArray &data) {
	QJsonObject root = QJsonDocument::fromJson(data).object();

	QJsonValue vObjects = root.value("objects");
	if (!vObjects.isObject()) {
		qCritical() << "Asset index. Incorrect objects";
	}
	QJsonObject objects = vObjects.toObject();

	this->assets.cdUp();
	if (!this->assets.exists("objects")) {
		qDebug() << "Objects dir not exist. Creating...";
		this->assets.mkdir("objects");
	}
	this->assets.cd("objects");

	for (auto i : objects) {
		if(!i.isObject()) {
			qCritical() << "Asset object incorrect type: " << i.type();
		}
		QJsonObject object = i.toObject();

		QJsonValue vHash = object.value("hash");
		if (!vHash.isString()) {
			qCritical() << "Asset hash. Incorrect type" << vHash.type();
		}

		QJsonValue vSize = object.value("size");
		if (!vSize.isDouble()) {
			qCritical() << "Asset size. Incorrect type" << vSize.type();
		}

		QString hash = vHash.toString();
		QString name = QString(hash[0].toLatin1()) + QString(hash[1].toLatin1()) + "/" + hash;
		QString url = "http://resources.download.minecraft.net/" + name;
		QString path = this->assets.filePath(name);

		DownloadFile *file = new DownloadFile(name, hash, path, url, QCryptographicHash::Sha1);
		this->files.push_back(file);
	}

	emit this->metaUpdated();
}

void AssetsDownloader::startDownload() {
	this->dw = new DownloadWorker();
	this->dw->setFileList(files);

	connect(this->dw, &DownloadWorker::onFinish, this, &AssetsDownloader::updated);
	connect(this->dw, &DownloadWorker::onDownloaded, this, &AssetsDownloader::updateProgress);
	//connect(this->dw, &DownloadWorker::onError, this, &AssetsDownloader::error);

	this->dw->start();
}

#include "update.h"

Update::Update(QObject *parent) : QObject(parent) {
	this->manager = new QNetworkAccessManager;
	this->branch = Config::config()->get("updates.branch", "master").toString();
	if (QSysInfo::kernelType() == "linux") {
		this->mainFile = LINUX_BIN;
	} else if (QSysInfo::kernelType() == "windows") {
		this->mainFile = WINDOWS_BIN;
	}
}

void Update::checkUpdate() {
	qDebug() << "Checking update...";
	connect(this->manager, &QNetworkAccessManager::finished, this, &Update::onResponse);
	QNetworkRequest request;
	request.setUrl(QString(API_SERVER)+ "launcher.updates");

	QUrlQuery post;
	post.addQueryItem("os", QSysInfo::kernelType());
	post.addQueryItem("branch", this->branch);

	QByteArray postData = post.toString(QUrl::FullyEncoded).toUtf8();
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	manager->post(request, postData);
}

void Update::onResponse(QNetworkReply *reply) {
	if (reply->error()) {
			qCritical() << "Update error: " << reply->errorString();
	} else {
		QJsonObject root = QJsonDocument::fromJson(reply->readAll()).object();
		QJsonValue vStatus = root.value("status");
		if (!vStatus.isObject()) {
			qCritical() << "Uncorrect server response: vStatus " << vStatus.type();
		}
		QJsonObject jStatus = vStatus.toObject();

		QJsonValue vStatusCode = jStatus.value("code");
		if (!vStatusCode.isDouble()) {
			qCritical() << "Uncorrect server response: vStatusCode " << vStatus.type();
		}
		int statusCode = vStatusCode.toInt();

		if (statusCode != 0) {
			QJsonValue vStatusMsg = jStatus.value("msg");
			if (!vStatusMsg.isString()) {
				qCritical() << "Uncorrect server response: vStatusMsg " << vStatusMsg.type();
			}
			qCritical() << "Error(" << statusCode << "): " << vStatusMsg.toString();
		}

		disconnect(this->manager, &QNetworkAccessManager::finished, this, &Update::onResponse);

		QJsonValue vBody = root.value("body");
		if (!vBody.isObject()) {
			qCritical() << "Uncorrect server response: vBody " << vBody.type();
		}
		this->parseBody(vBody.toObject());
	}
}

void Update::parseBody(QJsonObject body) {
	QJsonValue vUpdates = body.value("updates");
	if (!vUpdates.isArray()) {
		qCritical() << "Uncorrect server response: vUpdates " << vUpdates.type();
	}
	QJsonArray jUpdates = vUpdates.toArray();

	QList<DownloadFile *> files;
	for (int i = 0;i < jUpdates.size(); ++i) {
		if (!jUpdates[i].isObject()) {
			qCritical() << "Uncorrect server response: jUpdates " << jUpdates[i].type();
		}
		QJsonObject jUpdates_i = jUpdates[i].toObject();

		QJsonValue vFilename = jUpdates_i.value("filename");
		if (!vFilename.isString()) {
			qCritical() << "Uncorrect server response: vFilename " << vFilename.type();
		}

		QJsonValue vHash = jUpdates_i.value("hash");
		if (!vHash.isString()) {
			qCritical() << "Uncorrect server response: vHash " << vHash.type();
		}

		QString name, hash, path, uri;
		name = vFilename.toString();
		hash = vHash.toString();
		uri = LAUNCHER_MIRROR + QSysInfo::kernelType() + "/" + this->branch + "/" + name;
		path = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/" + name;

		DownloadFile *file = nullptr;
		if (name == this->mainFile) {
			DownloadFile *mFile = new DownloadFile(name, hash, path, uri, QCryptographicHash::Md5);
			if (!mFile->isCorrect()) {
				path += "_new";
				file = new DownloadFile(name, hash, path, uri, QCryptographicHash::Md5);
				files.append(file);
			}
			mFile->deleteLater();
		} else {
			file = new DownloadFile(name, hash, path, uri, QCryptographicHash::Md5);
			files.append(file);
		}
	}
	this->startDownload(files);
}

void Update::startDownload(const QList<DownloadFile *> &files) {
	this->dw = new DownloadWorker();
	this->dw->setFileList(files);
	connect(this->dw, &DownloadWorker::onFinish, this, &Update::downloadFinished);
	connect(this->dw, &DownloadWorker::onError, this, &Update::error);
	this->dw->start();
}

void Update::downloadFinished(quint64 countDownloadedFiles) {
	if (countDownloadedFiles > 0) {
		this->finishUpdate();
		this->restart();
	} else {
		emit this->updated();
	}
}

void Update::finishUpdate() {
	QString filePath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/" + this->mainFile;
	QFile inFile(filePath + "_new");
	QFile outFile(filePath);

	if (!inFile.exists())
		qCritical() << "Update not downloaded";
	else {
		outFile.remove();
		inFile.rename(this->mainFile);
		inFile.setPermissions(QFile::ExeGroup  | QFile::ExeOther  | QFile::ExeOther  | QFile::ExeUser |
							  QFile::ReadGroup | QFile::ReadOther | QFile::ReadOther | QFile::ReadUser |
							  QFile::WriteOwner);
	}
}

void Update::restart() {
	QString kernel = QSysInfo::kernelType();
	if (kernel == "linux") {
		QProcess *proc = new QProcess();
		proc->setWorkingDirectory(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
		proc->startDetached(LINUX_BIN);
		qApp->exit();
	}
}

void Update::error(QString msg) {
	qWarning() << msg;
}

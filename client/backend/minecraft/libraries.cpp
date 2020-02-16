#include "libraries.h"

Libraries::Libraries(QDir mc, QJsonArray libraries, QObject *parent) {
	this->libraries = mc;
	QString os = "";
	if (QSysInfo::kernelType() == "linux") {
		os = "linux";
	} else if (QSysInfo::kernelType() == "winnt") {
		os = "windows";
	}

	for (int i = 0; i < libraries.size(); ++i) {
		if (!libraries[i].isObject()) {
			qCritical() << "Libraries. Incorrect type: " << libraries[i].type();
		}
		QJsonObject lib = libraries[i].toObject();

		QJsonValue vDownloads = lib.value("downloads");
		if (!vDownloads.isObject()) {
			qCritical() << "Libraries. Downloads incorrect type: " << vDownloads.type();
		}
		QJsonObject downloads = vDownloads.toObject();

		QJsonValue vNatives = lib.value("natives");
		if (vNatives.isUndefined()) {
			QJsonValue vArtifact = downloads.value("artifact");
			if (vArtifact.isObject()) {
				this->libs.append(vArtifact);
			}
		} else if (vNatives.isObject()){
			QJsonObject natives = vNatives.toObject();

			QJsonValue vSystem = natives.value(os);
			if (vSystem.isUndefined()) {
				continue;
			}
			if (!vSystem.isString()) {
				qCritical() << "Libraries. Natives incorrect";
				break;
			}
			QString system = vSystem.toString();

			if (QSysInfo::currentCpuArchitecture() == "x86_64") {
				system = system.replace("${arch}", "64");
			} else {
				system = system.replace("${arch}", "32");
			}

			QJsonValue vClassifiers = downloads.value("classifiers");
			if (!vClassifiers.isObject()) {
				qCritical() << "Libraries. Classifiers incorrect";
				break;
			}
			QJsonObject classifiers = vClassifiers.toObject();

			QJsonValue vArtifact = classifiers.value(system);
			if (vArtifact.isUndefined()) {
				qDebug() << "Libraries. Mojang incorrect!";
				continue;
			}
			if (!vArtifact.isObject()) {
				qCritical() << "Libraries. Something wrong!";
				break;
			}
			this->libs.append(vArtifact);
		} else {
			qCritical() << "Libraries. Can`t parse JSON." << vNatives.type();
			break;
		}
	}
}

void Libraries::download() {
	this->dw = new DownloadWorker();

	if (!this->libraries.exists("libraries")) {
		qDebug() << "Libraries dir not exist. Creating...";
		this->libraries.mkdir("libraries");
	}
	this->libraries.cd("libraries");

	QList<DownloadFile *> files;
	for(int i = 0; i < this->libs.size(); ++i) {
		QJsonValue vLib = this->libs[i];
		if (!vLib.isObject()) {
			qCritical() << "Libraries. Download breaked: " << vLib.type();
			return;
		}
		QJsonObject lib = vLib.toObject();

		QJsonValue vPath = lib.value("path");
		if (!vPath.isString()) {
			qCritical() << "Libraries. Download path incorrect. " << vPath.type();
			return;
		}
		QJsonValue vSha1 = lib.value("sha1");
		if (!vSha1.isString()) {
			qCritical() << "Libraries. Download sha1 incorrect. " << vSha1.type();
			return;
		}
		QJsonValue vSize = lib.value("size");
		if (!vSize.isDouble()) {
			qCritical() << "Libraries. Download size incorrect. " << vSize.type();
			return;
		}
		QJsonValue vUrl = lib.value("url");
		if (!vUrl.isString()) {
			qCritical() << "Libraries. Download url incorrect. " << vUrl.type();
			return;
		}

		QString hash = vSha1.toString();
		QString name = vPath.toString();
		QString url = vUrl.toString();
		QString path = this->libraries.filePath(name);

		DownloadFile *file = new DownloadFile(name, hash, path, url, QCryptographicHash::Sha1);
		files.push_back(file);
	}

	this->dw->setFileList(files);

	connect(this->dw, &DownloadWorker::onFinish, this, &Libraries::downloaded);
	connect(this->dw, &DownloadWorker::onDownloaded, this, &Libraries::updateProgress);
	//connect(this->dw, &DownloadWorker::onError, this, &AssetsDownloader::error);

	this->dw->start();
}

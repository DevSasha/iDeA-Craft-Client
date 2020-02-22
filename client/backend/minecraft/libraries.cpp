#include "libraries.h"

Libraries::Libraries(QDir mc, QJsonArray libraries, QObject *parent) {
	this->libsDir = mc;

	if (QSysInfo::kernelType() == "linux") {
		this->os = "linux";
	} else if (QSysInfo::kernelType() == "winnt") {
		this->os = "windows";
	}

	if (QSysInfo::currentCpuArchitecture() == "x86_64") {
		this->arch = "64";
	} else {
		this->arch = "32";
	}

	for (int i = 0; i < libraries.size(); ++i) {
		if (!libraries[i].isObject()) {
			qCritical() << "Libraries. Incorrect type: " << libraries[i].type();
		}
		QJsonObject lib = libraries[i].toObject();
		this->addLibrary(lib);
		continue;
	}
}

Libraries::Libraries(QDir mc) {
	this->libsDir = mc;

	if (QSysInfo::kernelType() == "linux") {
		this->os = "linux";
	} else if (QSysInfo::kernelType() == "winnt") {
		this->os = "windows";
	}

	if (QSysInfo::currentCpuArchitecture() == "x86_64") {
		this->arch = "64";
	} else {
		this->arch = "32";
	}
}

Libraries::~Libraries() {

}

void Libraries::addLibrary(QJsonObject lib) {
	QJsonValue vLibName = lib.value("name");
	if (!vLibName.isString()) {
		qCritical() << "Libraries. Incorrect lib";
		return;
	}
	QString libName = vLibName.toString();

	QStringList list = libName.split(":");
	QString fullName = list[0] + "." + list[1],
			version = list[2];
	for (auto i : this->libraries) {
		vLibName = i.value("name");
		if (!vLibName.isString()) {
			qCritical() << "Libraries. Incorrect lib";
			return;
		}
		libName = vLibName.toString();
		list = libName.split(":");

		if (fullName != list[0] + "." + list[1]) {
			continue;
		}
		if (version == list[2]) {
			return;
		}

		QVersionNumber	Old = QVersionNumber::fromString(version),
						New = QVersionNumber::fromString(list[2]);
		if (Old < New) {
			this->libraries.remove(i);
			break;
		} else {
			return;
		}
	}
	this->libraries.push_back(lib);
}

QString Libraries::getClassPath() {
	QString classPath = "", delimer = "";
	if (QSysInfo::kernelType() == "linux") {
		delimer = ":";
	} else if (QSysInfo::kernelType() == "winnt") {
		delimer = ";";
	}
	for (auto i : this->classPaths) {
		classPath += i + delimer;
	}
	classPath.remove(classPath.size() - 1);
	return classPath;
}

void Libraries::addClassPath(QString path) {
	this->classPaths.push_back(path);
}

void Libraries::download() {
	if (!this->libsDir.exists("libraries")) {
		qDebug() << "Libraries dir not exist. Creating...";
		this->libsDir.mkdir("libraries");
	}
	this->libsDir.cd("libraries");

	QList<DownloadFile *> files;
	QJsonValue value, vArtifact;
	for(auto lib : this->libraries) {
		value = lib.value("downloads");
		if (!value.isObject()) {
			qCritical() << "Libraries. Downloads incorrect type: " << value.type();
		}
		QJsonObject downloads = value.toObject();

		value = lib.value("natives");
		if (value.isUndefined()) {
			value = downloads.value("artifact");
			if (value.isObject()) {
				vArtifact = value;
			}
		} else if (value.isObject()){
			QJsonObject natives = value.toObject();

			value = natives.value(os);
			if (value.isUndefined()) {
				continue;
			}
			if (!value.isString()) {
				qCritical() << "Libraries. Natives incorrect";
				break;
			}
			QString system = value.toString().replace("${arch}", this->arch);

			value = downloads.value("classifiers");
			if (!value.isObject()) {
				qCritical() << "Libraries. Classifiers incorrect";
				break;
			}
			QJsonObject classifiers = value.toObject();

			value = classifiers.value(system);
			if (value.isUndefined()) {
				qDebug() << "Libraries. Mojang incorrect!";
				continue;
			}
			if (!value.isObject()) {
				qCritical() << "Libraries. Something wrong!";
				break;
			}
			vArtifact = value;
		} else {
			qCritical() << "Libraries. Can`t parse JSON." << value.type();
			break;
		}

		if (!vArtifact.isObject()) {
			qCritical() << "Libraries. Artifact incorrect";
			return;
		}
		QJsonObject artifact = vArtifact.toObject();

		value = artifact.value("path");
		if (!value.isString()) {
			qCritical() << "Libraries. Download path incorrect. " << value.type();
			return;
		}
		QString name = value.toString();
		QString path = this->libsDir.filePath(name);
		this->classPaths.push_back(path);

		value = artifact.value("sha1");
		if (!value.isString()) {
			qCritical() << "Libraries. Download sha1 incorrect. " << value.type();
			return;
		}
		QString hash = value.toString();

		value = artifact.value("size");
		if (!value.isDouble()) {
			qCritical() << "Libraries. Download size incorrect. " << value.type();
			return;
		}

		value = artifact.value("url");
		if (!value.isString()) {
			qCritical() << "Libraries. Download url incorrect. " << value.type();
			return;
		}
		QString url = value.toString();

		DownloadFile *file = new DownloadFile(name, hash, path, url, QCryptographicHash::Sha1);
		files.push_back(file);
	}

	this->dw = new DownloadWorker();
	this->dw->setFileList(files);

	connect(this->dw, &DownloadWorker::onFinish, this, &Libraries::downloaded);
	connect(this->dw, &DownloadWorker::onDownloaded, this, &Libraries::onUpdateProgress);
	//connect(this->dw, &DownloadWorker::onError, this, &AssetsDownloader::error);

	this->dw->start();
}

void Libraries::onUpdateProgress(int progress) {
	emit this->updateProgress(progress);
}

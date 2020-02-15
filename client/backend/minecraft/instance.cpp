#include "instance.h"

Instance::Instance(int id) : QObject() {
	APIRequest *req = new APIRequest("server.info");
	req->addQueryItem("server_id", QString::number(id));
	connect(req, &APIRequest::finished, this, &Instance::replyServerInfo);
	req->send();
}

void Instance::replyServerInfo(QJsonObject *body) {
	QJsonValue vServer = body->value("server");
	delete body;

	QJsonObject server = vServer.toObject();
	this->name = server.value("title").toString();
	this->description = server.value("description").toString();
	this->version = new MinecraftVersion(server.value("version").toString());

	connect(this->version, &MinecraftVersion::updated, this, &Instance::versionUpdated);

	this->version->update();
}

void Instance::run() {
	connect(this->version, &MinecraftVersion::downloadProgress, this, &Instance::progressChanged);

	this->version->download();
}

void Instance::versionUpdated() {
	// NOTE Instance may be running before meta was update
}

void Instance::progressChanged(QString text, int progress) {
	emit this->updateProgress(text, progress);
}

QString Instance::getDescription() const {
	return description;
}

QString Instance::getName() const {
	return name;
}

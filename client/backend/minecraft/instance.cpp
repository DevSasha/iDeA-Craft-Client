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

	this->name = vServer.toObject().value("title").toString();
	this->description = vServer.toObject().value("description").toString();
}

void Instance::run() {

}

QString Instance::getDescription() const {
	return description;
}

QString Instance::getName() const {
	return name;
}

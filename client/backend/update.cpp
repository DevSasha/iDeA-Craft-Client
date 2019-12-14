#include "update.h"

Update::Update(QObject *parent) : QObject(parent) {
	this->manager = new QNetworkAccessManager;
}

void Update::checkUpdate() {
	qDebug() << "Checking update...";
	connect(this->manager, &QNetworkAccessManager::finished, this, &Update::onResponse);
	QNetworkRequest request;
	request.setUrl(QString(API_SERVER));
	manager->get(request);
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
			QJsonValue vStatusMsg = jStatus.value("mgs");
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

	QList<QString> filenames {}, filehashs {};
	for (int i = 0;i < jUpdates.size(); ++i) {
		if (!jUpdates[i].isObject()) {
			qCritical() << "Uncorrect server response: jUpdates " << jUpdates[i].type();
		}

		QJsonObject jUpdates_i = jUpdates[i].toObject();

		QJsonValue vFilename = jUpdates_i.value("filename");
		if (!vFilename.isArray()) {
			qCritical() << "Uncorrect server response: vFilename " << vFilename.type();
		}
		if (vFilename.toString() == "idea-craft") continue;
		filenames.append(vFilename.toString());

		QJsonValue vHash = jUpdates_i.value("hash");
		if (!vHash.isArray()) {
			qCritical() << "Uncorrect server response: vHash " << vHash.type();
		}
		filehashs.append(vHash.toString());
	}

}

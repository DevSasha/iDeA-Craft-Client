#include "apirequest.h"

std::vector<QString>
	APIRequest::defaultKeys = {},
	APIRequest::defaultValues = {};

APIRequest::APIRequest(QString method) : HTTPRequest(QString(API_SERVER) + method) {
	this->setup();

	ulong defaultKeysCount = defaultKeys.size();
	for (ulong i = 0; i < defaultKeysCount; ++i) {
		this->addQueryItem(this->defaultKeys[i], this->defaultValues[i]);
	}

	connect(this, &HTTPRequest::finished, this, &APIRequest::reply);
}

void APIRequest::addDefaultQueryItem(QString key, QString value) {
	defaultKeys.push_back(key);
	defaultValues.push_back(value);
}

void APIRequest::reply(QNetworkReply *reply) {
	if (reply->error()) {
		qCritical() << "API request error: " << reply->errorString();
		return;
	} else {
		QJsonObject root = QJsonDocument::fromJson(reply->readAll()).object();
		reply->deleteLater();

		QJsonValue vStatus = root.value("status");
		if (!vStatus.isObject()) {
			qDebug() << QJsonDocument(root).toJson();
			qCritical() << "Uncorrect server response: vStatus " << vStatus.type();
			return;
		}
		QJsonObject jStatus = vStatus.toObject();

		QJsonValue vStatusCode = jStatus.value("code");
		if (!vStatusCode.isDouble()) {
			qCritical() << "Uncorrect server response: vStatusCode " << vStatus.type();
			return;
		}
		int statusCode = vStatusCode.toInt();

		if (statusCode != 0) {
			QJsonValue vStatusMsg = jStatus.value("msg");
			if (!vStatusMsg.isString()) {
				qCritical() << "Uncorrect server response: vStatusMsg " << vStatusMsg.type();
				return;
			}
			qWarning() << "Error(" << statusCode << "): " << vStatusMsg.toString();
		}

		QJsonValue vBody = root.value("body");
		if (!vBody.isObject()) {
			qCritical() << "Uncorrect server response: vBody " << vBody.type();
			return;
		}
		QJsonObject *body = new QJsonObject(vBody.toObject());
		emit this->finished(body);
	}
}

#include "apirequest.h"

std::vector<QString>
	APIRequest::defaultKeys = {},
	APIRequest::defaultValues = {};

APIRequest::APIRequest(QString method) : QObject () {
	if (this->mng == nullptr) {
		this->mng = new QNetworkAccessManager;
	}

	this->method = method;

	ulong defaultKeysCount = defaultKeys.size();
	for (ulong i = 0; i < defaultKeysCount; ++i) {
		this->addQueryItem(this->defaultKeys[i], this->defaultValues[i]);
	}

	connect(this->mng, &QNetworkAccessManager::finished, this, &APIRequest::reply);
}

APIRequest::~APIRequest() {
	delete this->mng;
}

void APIRequest::addQueryItem(QString key, QString value) {
	this->form.addQueryItem(key, value);
}

void APIRequest::addDefaultQueryItem(QString key, QString value) {
	this->defaultKeys.push_back(key);
	this->defaultValues.push_back(value);
}

void APIRequest::send() {
	QNetworkRequest request;
	request.setUrl(QString(API_SERVER)+ this->method);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	QByteArray postData = this->form.toString(QUrl::FullyEncoded).toUtf8();
	this->mng->post(request, postData);
}

void APIRequest::reply(QNetworkReply *reply) {
	if (reply->error()) {
			qCritical() << "Update error: " << reply->errorString();
	} else {
		QJsonObject root = QJsonDocument::fromJson(reply->readAll()).object();
		reply->deleteLater();

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
			qWarning() << "Error(" << statusCode << "): " << vStatusMsg.toString();
		}

		QJsonValue vBody = root.value("body");
		if (!vBody.isObject()) {
			qCritical() << "Uncorrect server response: vBody " << vBody.type();
		}
		QJsonObject *body = new QJsonObject(vBody.toObject());
		emit this->finished(body);
		this->deleteLater();
	}
}

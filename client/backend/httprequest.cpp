#include "httprequest.h"

QNetworkAccessManager *HTTPRequest::mng = nullptr;
std::queue<HTTPRequest *> HTTPRequest::query {};

HTTPRequest::HTTPRequest(QString url, QString method) {
	setup();
	this->url = url;
	this->method = method;
}

HTTPRequest::~HTTPRequest() {

}

void HTTPRequest::addQueryItem(QString key, QString value) {
	this->form.addQueryItem(key, value);
}

void HTTPRequest::send() {
	request.setUrl(this->url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

	this->query.push(this);

	if (this->query.size() == 1) {
		this->_send();
	}
}

void HTTPRequest::reply(QNetworkReply *reply) {
	this->close();
	emit this->finished(reply);
}

void HTTPRequest::setup() {
	if (this->mng == nullptr) {
		this->mng = new QNetworkAccessManager;
	}
}

void HTTPRequest::_send() {
	connect(this->mng, &QNetworkAccessManager::finished, this, &HTTPRequest::reply);\

	if (this->method == "POST") {
		this->postData = this->form.toString(QUrl::FullyEncoded).toUtf8();
		this->mng->post(request, postData);
	} else if (this->method == "GET") {
		this->mng->get(request);
	}
}

void HTTPRequest::close() {
	disconnect(this->mng, &QNetworkAccessManager::finished, this, &HTTPRequest::reply);
	this->query.pop();
	if (this->query.size() > 0) {
		this->query.front()->_send();
	}
	this->deleteLater();
}

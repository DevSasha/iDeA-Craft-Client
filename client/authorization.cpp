#include "authorization.h"

Authorization::Authorization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authorization)
{
	this->manager = new QNetworkAccessManager;

    ui->setupUi(this);
	setWindowTitle("Вход");
	setFixedSize(466, 307);
}

Authorization::~Authorization(){
	delete this->ui;
	delete this->manager;
}

void Authorization::auth() {
	connect(manager, &QNetworkAccessManager::finished, this, &Authorization::authReply);
	QNetworkRequest request;
	request.setUrl(QString(API_SERVER)+ "user.auth");

	QUrlQuery post;
	post.addQueryItem("nickname", login);
	post.addQueryItem("step", "1");

	QByteArray postData = post.toString(QUrl::FullyEncoded).toUtf8();
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	manager->post(request, postData);
}

void Authorization::authStep2(QString salt) {
	connect(this->manager, &QNetworkAccessManager::finished, this, &Authorization::authStep2Reply);
	QNetworkRequest request;
	request.setUrl(QString(API_SERVER)+ "user.auth");

	QUrlQuery post;
	post.addQueryItem("nickname", login);
	post.addQueryItem("step", "2");

	QString md5Password = QCryptographicHash::hash(this->password.toUtf8(), QCryptographicHash::Md5).toHex();
	QString doubleMd5Password = QCryptographicHash::hash(md5Password.toUtf8(), QCryptographicHash::Md5).toHex();
	QString password = QCryptographicHash::hash((doubleMd5Password.toUtf8() + salt).toUtf8(), QCryptographicHash::Sha512).toHex();
	post.addQueryItem("password", password);

	QByteArray postData = post.toString(QUrl::FullyEncoded).toUtf8();
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	manager->post(request, postData);
}

void Authorization::on_login_button_clicked() {
	if (this->isSigninMode) {
		this->isSigninMode = false;
		this->emailField->deleteLater();
	} else {
		this->login = ui->login_field->text();
		this->password = ui->password_field->text();

		if (checkLoginPasswd()) {
			auth();
		}
	}
}

void Authorization::on_signin_button_clicked() {
	if(!this->isSigninMode){
		this->isSigninMode = true;
		this->emailField = new QLineEdit(ui->layoutWidget1);
		this->emailField->setPlaceholderText("E-Mail");
		ui->verticalLayout->insertWidget(0, this->emailField);
    }else {
		this->login = ui->login_field->text();
		this->password = ui->password_field->text();

		if (checkLoginPasswd()) {
			reg(this->login, this->emailField->text(), this->password);
		}
    }
}

void Authorization::on_isSafe_stateChanged(int arg1)
{
	this->safePass = arg1;
}

void Authorization::regReply(QNetworkReply *reply) {
	disconnect(manager, &QNetworkAccessManager::finished, this, &Authorization::regReply);
	if(reply->error()){
			qCritical() << "ERROR" << reply->errorString();
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

		auth();
	}
}

void Authorization::authReply(QNetworkReply *reply) {
	disconnect(manager, &QNetworkAccessManager::finished, this, &Authorization::authReply);
	if(reply->error()){
			qCritical() << "ERROR" << reply->errorString();
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

		QJsonValue vBody = root.value("body");
		if (!vBody.isObject()) {
			qCritical() << "Uncorrect server response: vBody " << vBody.type();
		}
		QJsonObject jBody = vBody.toObject();

		QJsonValue vSalt = jBody.value("salt");
		if (!vSalt.isString()) {
			qCritical() << "Uncorrect server response: vSalt " << vSalt.type();
		}
		QString salt = vSalt.toString();

		authStep2(salt);
	}
}

void Authorization::authStep2Reply(QNetworkReply *reply) {
	disconnect(manager, &QNetworkAccessManager::finished, this, &Authorization::authStep2Reply);
	if(reply->error()){
		qCritical() << "ERROR" << reply->errorString();
		return;
	}
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

	QJsonValue vBody = root.value("body");
	if (!vBody.isObject()) {
		qCritical() << "Uncorrect server response: vBody " << vBody.type();
	}
	QJsonObject jBody = vBody.toObject();

	QJsonValue vSessionToken = jBody.value("sessionToken");
	if (!vSessionToken.isObject()) {
		qCritical() << "Uncorrect server response: vSessionToken " << vSessionToken.type();
	}
	this->sessionToken = vSessionToken.toString();

	qDebug() << "authorized";
	if(this->safePass){
		Config *cfg = Config::config();
		cfg->save("auth.login", this->login);
		cfg->save("auth.password", this->password);
	}

	emit this->authorized();
}

bool Authorization::checkLoginPasswd() {
	if (this->login.size() == 0) {
		if (this->password.size() == 0) qWarning() << "Вы не ввели логин и пароль.";
		else qWarning() << "Вы не ввели логин.";
	} else {
		if (this->password.size() == 0) qWarning() << "Вы не ввели пароль.";
		else return true;
	}
	return false;
}

void Authorization::reg(QString login, QString email, QString password) {
	connect(this->manager, &QNetworkAccessManager::finished, this, &Authorization::regReply);
	QNetworkRequest request;
	request.setUrl(QString(API_SERVER)+ "user.reg");

	QUrlQuery post;
	post.addQueryItem("nickname", login);
	post.addQueryItem("email", email);
	post.addQueryItem("password", password);

	QByteArray postData = post.toString(QUrl::FullyEncoded).toUtf8();
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	manager->post(request, postData);
}

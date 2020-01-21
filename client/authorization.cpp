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

void Authorization::on_login_button_clicked() {
	if (this->isSigninMode) {
		this->isSigninMode = false;
		this->nikname->deleteLater();
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
		this->nikname = new QLineEdit(ui->layoutWidget1);
		this->nikname->setPlaceholderText("Ник");
        ui->verticalLayout->insertWidget(0, nikname);
    }else {
		this->nik = nikname->text();
		this->login = ui->login_field->text();
		this->password = ui->password_field->text();
    }
}

void Authorization::read(QByteArray msg)
{
    QJsonDocument doc = QJsonDocument::fromBinaryData(msg);
    QJsonObject root = doc.object();
    QJsonValue method = root.value("method");
    QString strMethod;
    if(method.isString()) strMethod = method.toString(); else return;

         if(strMethod == "registration") signIn(&root);
    else if(strMethod == "authorization") logIn(&root);
         else qDebug("Err");
}

int Authorization::logIn(QJsonObject *obj)
{
    QString response = "";
    QJsonValue res = obj->value("response");
    if(res.isString()) response = res.toString();
    if(response == "sucsess"){
        nik = obj->value("nikname").toString();
        qDebug() << "authorized";
        //safePass = true;
        emit authorized();
        //TODO: logIn sucsess
        if(this->safePass){
            Config *cfg = Config::config();
            cfg->save("auth.login", this->login);
            cfg->save("auth.password", this->password);
        }
    }
    else {
        return -1;
    }
    return 0;
}

int Authorization::signIn(QJsonObject *obj)
{
    QString response = "";
    QJsonValue res = obj->value("response");
    if(res.isString()) response = res.toString();
    if(response == "sucsess"){
        qDebug() << "Sucsess";
        auth();
    }
    else {
        return -1;
    }
    return 0;
}

void Authorization::on_isSafe_stateChanged(int arg1)
{
	this->safePass = arg1;
}

void Authorization::regReply(QNetworkReply *reply) {

}

void Authorization::authReply(QNetworkReply *reply) {
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
	}
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

#include "authorization.h"

Authorization::Authorization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authorization)
{
    ui->setupUi(this);
	setWindowTitle("Вход");
	setFixedSize(466, 307);
}

Authorization::~Authorization(){
	delete this->ui;
}

QString Authorization::getSessionToken() {
	return this->sessionToken;
}

void Authorization::auth() {
	APIRequest *req = new APIRequest("user.auth");
	connect(req, &APIRequest::finished, this, &Authorization::authReply);

	req->addQueryItem("nickname", login);
	req->addQueryItem("step", "1");

	req->send();
}

void Authorization::authStep2(QString salt) {
	APIRequest *req = new APIRequest("user.auth");
	connect(req, &APIRequest::finished, this, &Authorization::authStep2Reply);

	req->addQueryItem("nickname", login);
	req->addQueryItem("step", "2");

	QString md5Password = QCryptographicHash::hash(this->password.toUtf8(), QCryptographicHash::Md5).toHex();
	QString doubleMd5Password = QCryptographicHash::hash(md5Password.toUtf8(), QCryptographicHash::Md5).toHex();
	QString password = QCryptographicHash::hash((doubleMd5Password.toUtf8() + salt).toUtf8(), QCryptographicHash::Sha512).toHex();
	req->addQueryItem("password", password);

	req->send();
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

void Authorization::regReply(QJsonObject *body) {
	QJsonValue vUserId = body->value("user_id");
	delete body;

	if (!vUserId.isDouble()) {
		qCritical() << "Uncorrect server response: vUserId " << vUserId.type();
	}
	qDebug() << "User registered with id " << vUserId.toInt();

	auth();
}

void Authorization::authReply(QJsonObject *body) {
	QJsonValue vSalt = body->value("salt");
	delete body;

	if (!vSalt.isString()) {
		qCritical() << "Uncorrect server response: vSalt " << vSalt.type();
	}
	QString salt = vSalt.toString();
	authStep2(salt);
}

void Authorization::authStep2Reply(QJsonObject *body) {
	QJsonValue vSessionToken = body->value("sessionToken");
	delete body;

	if (!vSessionToken.isString()) {
		qCritical() << "Uncorrect server response: vSessionToken " << vSessionToken.type();
	}
	this->sessionToken = vSessionToken.toString();

	qDebug() << "authorized";
	if(this->safePass) {
		Config *cfg = Config::config();
		cfg->save("auth.login", this->login);
		cfg->save("auth.password", this->password);
	}

	emit this->authorized();
}

bool Authorization::checkLoginPasswd() {
	if (this->login == this->password) {
		qWarning() << "Логин и пароль не могут быть идентичными.";
		return false;
	}
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
	APIRequest *req = new APIRequest("user.reg");
	connect(req, &APIRequest::finished, this, &Authorization::regReply);

	req->addQueryItem("nickname", login);
	req->addQueryItem("email", email);
	req->addQueryItem("password", password);

	req->send();
}

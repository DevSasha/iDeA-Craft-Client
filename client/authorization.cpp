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

int Authorization::auth()
{
    if(!login.size() && password.size()) QMessageBox::warning(this, "Ошибка", "Вы не ввели логин");
    if(login.size() && !password.size()) QMessageBox::warning(this, "Ошибка", "Вы не ввели пароль");
    if(!login.size() && !password.size()) QMessageBox::warning(this, "Ошибка", "Вы не ввели логин и пароль");
    if(login.size() && password.size()){
        QJsonObject root;
        root.insert("lock", "");
        root.insert("method", "authorization");
        root.insert("version", "1");
        root.insert("login", login);
        root.insert("password", password);
        QJsonDocument doc(root);
    }
    return 0;
}

void Authorization::on_login_button_clicked()
{
    if(SignIn){
        SignIn = false;
        nikname->deleteLater();
    }
    else {
        login = ui->login_field->text();
        password = ui->password_field->text();
        auth();
    }
}

void Authorization::on_signin_button_clicked()
{
    if(!SignIn){
        SignIn = true;
        nikname = new QLineEdit(ui->layoutWidget1);
        nikname->setPlaceholderText("Ник");
        ui->verticalLayout->insertWidget(0, nikname);
    }else {
        nik = nikname->text();
        login = ui->login_field->text();
        password = ui->password_field->text();

        if(!login.size() && password.size()) QMessageBox::warning(this, "Ошибка", "Вы не ввели логин");
        if(login.size() && !password.size()) QMessageBox::warning(this, "Ошибка", "Вы не ввели пароль");
        if(!login.size() && !password.size()) QMessageBox::warning(this, "Ошибка", "Вы не ввели логин и пароль");
        if(login.size() && password.size() && nik.size()){
            QJsonObject root;
            root.insert("lock", "");
            root.insert("method", "registration");
            root.insert("version", "1");
            root.insert("nikname", nik);
            root.insert("login", login);
            root.insert("password", password);
            QJsonDocument doc(root);
        }
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
    safePass = arg1;

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

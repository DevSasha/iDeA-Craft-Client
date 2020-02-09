#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QMessageBox>
#include <QDebug>
#include <backend/config.h>
#include <backend/apirequest.h>
#include <QJsonObject>
#include "ui_authorization.h"
#include "defines.h"

namespace Ui {
    class Authorization;
}

class Authorization : public QDialog
{
    Q_OBJECT

public:
    explicit Authorization(QWidget *parent = nullptr);
    ~Authorization();

	QString login;
    bool safePass = false;

	QString getSessionToken();

signals:
    void authorized();

private slots:
	void on_login_button_clicked();
	void on_signin_button_clicked();
    void on_isSafe_stateChanged(int arg1);

	void regReply(QJsonObject *body);
	void authReply(QJsonObject *body);
	void authStep2Reply(QJsonObject *body);

private:
	bool checkLoginPasswd();

	void reg(QString login, QString email, QString password);
	void auth();
	void authStep2(QString salt);

    Ui::Authorization *ui;
	QLineEdit *emailField;
	QString password, sessionToken;

	bool isSigninMode = false;
};

#endif // AUTHORIZATION_H

#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QMessageBox>
#include <backend/config.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
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

    QString nik, login, password;
    bool safePass = false;

signals:
    void authorized();

private slots:
	void on_login_button_clicked();
	void on_signin_button_clicked();
    void on_isSafe_stateChanged(int arg1);

	void regReply(QNetworkReply *reply);
	void authReply(QNetworkReply *reply);

private:
	bool checkLoginPasswd();

	void reg(QString login, QString email, QString password);
	void auth();

    Ui::Authorization *ui;
    QLineEdit *nikname;
	QNetworkAccessManager *manager;

	bool isSigninMode = false;
};

#endif // AUTHORIZATION_H

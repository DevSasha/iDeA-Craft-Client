#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QDialog>
#include <QMessageBox>
#include "client.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "ui_authorization.h"

namespace Ui {
    class Authorization;
}

class Authorization : public QDialog
{
    Q_OBJECT

public:
    explicit Authorization(Client *client, QWidget *parent = nullptr);
    ~Authorization();
    int auth();

    QString nik, login, password;
    bool safePass = false;

signals:
    void authorized();

private slots:
    void on_login_button_clicked();
    void on_signin_button_clicked();
    void read(QByteArray msg);

    void on_isSafe_stateChanged(int arg1);

private:
    int logIn(QJsonObject *obj);
    int signIn(QJsonObject *obj);

    Ui::Authorization *ui;
    QLineEdit *nikname;
    bool SignIn = false;
    Client *client;
};

#endif // AUTHORIZATION_H

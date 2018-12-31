#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QDialog>
#include <QMainWindow>
#include <QMessageBox>
#include "client.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "ui_authorization.h"

namespace Ui {
    class Authorization;
}

class Authorization : public QMainWindow
{
    Q_OBJECT

public:
    explicit Authorization(Client *client, QWidget *parent = nullptr);
    ~Authorization();
    int logIn();

    QString nik, login, password;
    Client *client;
private slots:
    void on_login_button_clicked();
    void on_signin_button_clicked();

    void read(QByteArray msg);

private:
    Ui::Authorization *ui;
    QLineEdit *nikname;
    bool SignIn = false;
};

#endif // AUTHORIZATION_H

#include "mainwindow.h"
#include "authorization.h"
#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //QHostAddress host("188.18.154.124");
    QApplication a(argc, argv);
    Client client;

    Authorization window_auth(&client);
    window_auth.show();

    //MainWindow window_main;
    //window_main.show();

    return a.exec();
}


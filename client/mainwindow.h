#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSysInfo>
#include <QJsonArray>
#include <backend/apirequest.h>
#include <backend/instance.h>
#include <ui/instancecard.h>
#include <defines.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
	void setNickname(QString nick);

signals:


public slots:
	void refreshServerListReply(QJsonObject *body);

private slots:


private:
    Ui::MainWindow *ui;
	QString nickname = "", sessionToken;

	void refreshServerList();
};

#endif // MAINWINDOW_H

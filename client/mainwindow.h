#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSysInfo>
#include <QJsonArray>
#include <backend/apirequest.h>
#include <ui/instancecard.h>
#include <ui/instancepanel.h>
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
	void changeInstance(InstancePanel *panel);

private slots:


private:
    Ui::MainWindow *ui;
	InstancePanel *currentPanel;
	QString nickname = "", sessionToken;

	void refreshServerList();
};

#endif // MAINWINDOW_H

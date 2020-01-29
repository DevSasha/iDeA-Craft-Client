#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSysInfo>
#include <backend/apirequest.h>
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


private slots:


private:
    Ui::MainWindow *ui;
	QString nickname, sessionToken;
};

#endif // MAINWINDOW_H

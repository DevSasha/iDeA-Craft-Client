#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QSysInfo>
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
    void setNik(QString nik);


signals:
    void startInst(QString name);

public slots:
    void processUpdate(int status);
    void installationComplete();

private slots:
    void on_run_clicked();

private:
    Ui::MainWindow *ui;
    QString nik;
};

#endif // MAINWINDOW_H

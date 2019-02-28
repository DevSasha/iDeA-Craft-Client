#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

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
    void startMC(QString);
    void downloadInst(QString name);
    void startInst(QString name);

public slots:
    void processUpdate(int status);
    void installationComplete();

private slots:
    void on_run_clicked();

private:
    Ui::MainWindow *ui;
    QString nik;
    bool installedMC = false;
};

#endif // MAINWINDOW_H

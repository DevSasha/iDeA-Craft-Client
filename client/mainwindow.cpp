#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("iDeA-Craft Launcher");
}

MainWindow::~MainWindow()
{
    delete ui;
    qDebug() << "Close";
}

void MainWindow::setNik(QString nik)
{
    this->nik = nik;
    ui->statusBar->showMessage("v0.0.2.10    Authorized as " + nik);
}

void MainWindow::processUpdate(int status)
{
    ui->progressBar->setValue(status);
}

void MainWindow::installationComplete()
{
    ui->label->setText("<html><head/><body><p><span style=\" color:#00c800;\">Статус: Майнкрафт установлен</span></p></body></html>");
}

void MainWindow::on_run_clicked()
{
    emit startInst("lite7");
    qDebug() << "On run clicked";
}

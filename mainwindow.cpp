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
    ui->statusBar->showMessage("Authorized as " + nik);
}

void MainWindow::processUpdate(int status)
{
    ui->progressBar->setValue(status);
}

void MainWindow::installationComplete()
{
    installedMC = true;
    ui->label->setText("<html><head/><body><p><span style=\" color:#00c800;\">Статус: Майнкрафт установлен</span></p></body></html>");
}

void MainWindow::on_run_clicked()
{
    if(installedMC){
        emit startInst("first");
    }else {
        emit downloadInst("first");
    }
}

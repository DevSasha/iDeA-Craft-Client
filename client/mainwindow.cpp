#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	setWindowTitle(WINDOW_TITLE);
}

MainWindow::~MainWindow() {
    delete ui;
	qDebug() << "Close";
}

void MainWindow::setNickname(QString nick) {
	this->nickname = nick;
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

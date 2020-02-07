#include "instancepanel.h"
#include "ui_instancepanel.h"

InstancePanel::InstancePanel(QString name, QString description) :
	QWidget(),
	ui(new Ui::InstancePanel)
{
	ui->setupUi(this);
	ui->title->setText(name);
	ui->description->setHtml(description);
	ui->progressBar->hide();
}

InstancePanel::~InstancePanel() {
	delete ui;
}

void InstancePanel::showProgress(bool state) {
	if (state)
		ui->progressBar->show();
	else
		ui->progressBar->hide();
}

void InstancePanel::updateProgress(QString text, int progress) {
	// TODO Process description
	ui->progressBar->setValue(progress);
}

void InstancePanel::on_runInstance_clicked() {
	emit this->run();
}

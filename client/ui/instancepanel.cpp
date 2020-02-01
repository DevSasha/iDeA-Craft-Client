#include "instancepanel.h"
#include "ui_instancepanel.h"

InstancePanel::InstancePanel(QString name, QString description) :
	QWidget(),
	ui(new Ui::InstancePanel)
{
	ui->setupUi(this);
	ui->title->setText(name);
	ui->description->setHtml(description);
}

InstancePanel::~InstancePanel() {
	delete ui;
}

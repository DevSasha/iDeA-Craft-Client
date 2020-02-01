#include "instancepanel.h"
#include "ui_instancepanel.h"

InstancePanel::InstancePanel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::InstancePanel)
{
	ui->setupUi(this);
}

InstancePanel::~InstancePanel()
{
	delete ui;
}

#include "instancecard.h"
#include "ui_instansecard.h"

InstanceCard::InstanceCard(int id, QString name) :
	QWidget(),
	ui(new Ui::InstanceCard)
{
	ui->setupUi(this);
	ui->serverName->setText(name);
}

InstanceCard::~InstanceCard()
{
	delete ui;
}

void InstanceCard::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {

	}
 }

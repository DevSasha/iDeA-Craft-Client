#include "instancecard.h"
#include "ui_instancecard.h"

InstanceCard::InstanceCard(int id, QString name, QString imgUri) :
	QWidget(),
	ui(new Ui::InstanceCard)
{
	this->id = id;
	ui->setupUi(this);
	ui->serverName->setText(name);
}

InstanceCard::~InstanceCard() {
	delete ui;
}

void InstanceCard::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		if (this->panel != nullptr) {
			emit this->clicked(this->panel);
		}
	}
}

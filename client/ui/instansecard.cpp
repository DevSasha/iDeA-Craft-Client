#include "instansecard.h"
#include "ui_instansecard.h"

InstanseCard::InstanseCard(int id, QString name) :
	QWidget(),
	ui(new Ui::InstanseCard)
{
	ui->setupUi(this);
	ui->serverName->setText(name);
}

InstanseCard::~InstanseCard()
{
	delete ui;
}

void InstanseCard::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {

	}
 }

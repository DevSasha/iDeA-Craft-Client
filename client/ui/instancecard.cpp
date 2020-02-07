#include "instancecard.h"
#include "ui_instancecard.h"

InstanceCard::InstanceCard(int id, QString name, QString imgUri) :
	QWidget(),
	ui(new Ui::InstanceCard)
{
	this->id = id;
	ui->setupUi(this);
	ui->serverName->setText(name);

	this->mng = new QNetworkAccessManager;
	connect(this->mng, &QNetworkAccessManager::finished, this, &InstanceCard::getIcon);

	this->instance = new Instance(id);
}

InstanceCard::~InstanceCard() {
	delete ui;
}

void InstanceCard::getIcon(QNetworkReply *reply) {
	if (reply->error()) {
		qWarning() << "Couldn`t get server icon";
	} else {
		// TODO Read reply and add to QGraphics View
	}
}

void InstanceCard::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		InstancePanel *panel = new InstancePanel(this->instance->getName(), this->instance->getDescription());

		connect(panel, &InstancePanel::run, this->instance, &Instance::run);
		connect(this->instance, &Instance::updateProgress, panel, &InstancePanel::updateProgress);
		connect(this->instance, &Instance::showProgressBar, panel, &InstancePanel::showProgress);

		emit this->clicked(panel);
	}
}

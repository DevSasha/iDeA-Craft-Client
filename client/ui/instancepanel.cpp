#include "instancepanel.h"
#include "ui_instancepanel.h"

InstancePanel::InstancePanel(int id) :
	QWidget(),
	ui(new Ui::InstancePanel)
{
	ui->setupUi(this);

	APIRequest *req = new APIRequest("server.info");
	req->addQueryItem("server_id", QString::number(id));
	connect(req, &APIRequest::finished, this, &InstancePanel::replyServerInfo);
	req->send();
}

InstancePanel::~InstancePanel() {
	delete ui;
}

void InstancePanel::replyServerInfo(QJsonObject *body) {
	QJsonValue vServer = body->value("server");
	delete body;

	QJsonValue vTitle = vServer.toObject().value("title");
	QJsonValue vDescription = vServer.toObject().value("description");
	ui->title->setText(vTitle.toString());
	ui->description->setHtml(vDescription.toString());
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	setWindowTitle(WINDOW_TITLE);

	refreshServerList();
}

MainWindow::~MainWindow() {
    delete ui;
	qDebug() << "Close";
}

void MainWindow::setNickname(QString nick) {
	if (this->nickname != "") return;
	this->nickname = nick;
}

void MainWindow::refreshServerListReply(QJsonObject *body) {
	QJsonValue vList = body->value("list");
	delete body;

	if (!vList.isArray()) {
		qCritical() << "Uncorrect server response: vList " << vList.type();
	}
	QJsonArray list = vList.toArray();

	for (int i = 0; i < list.size(); ++i) {
		QJsonObject server = list[i].toObject();

		int id = server.value("id").toInt();
		QString title = server.value("title").toString();
		QString imgUri = server.value("img_uri").toString();

		InstanceCard *card = new InstanceCard(id, title, imgUri);

		ui->instanseList->addWidget(card);
	}
}

void MainWindow::refreshServerList() {
	APIRequest *req = new APIRequest("server.list");
	connect(req, &APIRequest::finished, this, &MainWindow::refreshServerListReply);
	req->send();
}

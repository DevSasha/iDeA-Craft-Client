#ifndef INSTANSECARD_H
#define INSTANSECARD_H

#include <QWidget>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <backend/minecraft/instance.h>
#include <ui/instancepanel.h>

namespace Ui {
class InstanceCard;
}

class InstanceCard : public QWidget {
	Q_OBJECT

public:
	explicit InstanceCard(int id, QString name, QString imgUri);
	~InstanceCard();

public slots:
	void getIcon(QNetworkReply *reply);

signals:
	void clicked(InstancePanel *);

protected:
	void mousePressEvent(QMouseEvent *event);

private:
	int id;
	Ui::InstanceCard *ui;
	Instance *instance;
	QNetworkAccessManager *mng;
};

#endif // INSTANSECARD_H

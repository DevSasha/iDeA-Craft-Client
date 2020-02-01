#ifndef INSTANSECARD_H
#define INSTANSECARD_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class InstanceCard;
}

class InstanceCard : public QWidget {
	Q_OBJECT

public:
	explicit InstanceCard(int id, QString name);
	~InstanceCard();

protected:
	void mousePressEvent(QMouseEvent *event);

private:
	Ui::InstanceCard *ui;
};

#endif // INSTANSECARD_H

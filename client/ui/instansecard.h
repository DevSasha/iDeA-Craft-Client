#ifndef INSTANSECARD_H
#define INSTANSECARD_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class InstanseCard;
}

class InstanseCard : public QWidget {
	Q_OBJECT

public:
	explicit InstanseCard(int id, QString name);
	~InstanseCard();

protected:
	void mousePressEvent(QMouseEvent *event);

private:
	Ui::InstanseCard *ui;
};

#endif // INSTANSECARD_H

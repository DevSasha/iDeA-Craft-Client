#ifndef INSTANCEPANEL_H
#define INSTANCEPANEL_H

#include <QWidget>

namespace Ui {
class InstancePanel;
}

class InstancePanel : public QWidget
{
	Q_OBJECT

public:
	explicit InstancePanel(QWidget *parent = 0);
	~InstancePanel();

private:
	Ui::InstancePanel *ui;
};

#endif // INSTANCEPANEL_H

#ifndef INSTANCEPANEL_H
#define INSTANCEPANEL_H

#include <QWidget>
#include <backend/apirequest.h>

namespace Ui {
class InstancePanel;
}

class InstancePanel : public QWidget {
	Q_OBJECT
public:
	explicit InstancePanel(QString name, QString description);
	~InstancePanel();

public slots:

signals:

private:
	Ui::InstancePanel *ui;
};

#endif // INSTANCEPANEL_H

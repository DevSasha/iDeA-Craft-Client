#ifndef INSTANCE_H
#define INSTANCE_H

#include <QObject>

class Instance : public QObject {
	Q_OBJECT
public:
	explicit Instance(int id);

signals:

public slots:
};

#endif // INSTANCE_H

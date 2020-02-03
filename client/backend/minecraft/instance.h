#ifndef INSTANCE_H
#define INSTANCE_H

#include <QObject>
#include <backend/apirequest.h>
#include <backend/minecraft/minecraftversion.h>

class Instance : public QObject {
	Q_OBJECT
public:
	explicit Instance(int id);

	QString getName() const;
	QString getDescription() const;

signals:

public slots:
	void replyServerInfo(QJsonObject *body);
	void run();

private:
	QString name;
	QString description;
	QString ip;

	MinecraftVersion *version;
};

#endif // INSTANCE_H

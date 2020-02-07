#ifndef INSTANCE_H
#define INSTANCE_H

#include <QObject>
#include <backend/apirequest.h>
#include <backend/minecraft/minecraftversion.h>
#include <backend/minecraft/assetsdownloader.h>

class Instance : public QObject {
	Q_OBJECT
public:
	explicit Instance(int id);

	QString getName() const;
	QString getDescription() const;

signals:
	void updateProgress(QString, int);
	void showProgressBar(bool);

public slots:
	void replyServerInfo(QJsonObject *body);
	void run();

	void assetsProgress(int progress);
	void assetsUpdated();

private:
	QString name;
	QString description;
	QString ip;

	MinecraftVersion *version;
	AssetsDownloader *assets;
};

#endif // INSTANCE_H

#ifndef MINECRAFTVANILA_H
#define MINECRAFTVANILA_H

#include <QObject>
#include <backend/minecraft/minecraftbase.h>

class MinecraftVanila : public MinecraftBase{
	Q_OBJECT
public:
	MinecraftVanila(QString version);

	void update() override;
	void download() override;
	void assetDownloadFinish() override;
};

#endif // MINECRAFTVANILA_H

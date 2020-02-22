#include "minecraftvanila.h"

MinecraftVanila::MinecraftVanila(QString version) : MinecraftBase(version) {

}

void MinecraftVanila::update() {
	HTTPRequest *req = new HTTPRequest("https://launchermeta.mojang.com/v1/packages/2e818dc89e364c7efcfa54bec7e873c5f00b3840/1.7.10.json");
	connect(req, &HTTPRequest::finished, this, &MinecraftBase::replyVersionMeta);
	req->send();
}

void MinecraftVanila::download() {
	this->libs = new Libraries(this->dir, this->libraries);
	this->assets = new AssetsDownloader(this->dir, this->assetIndex);

	connect(this->assets, &AssetsDownloader::metaUpdated, this, &MinecraftBase::assetDownloadStart);

	this->assets->update();
}

void MinecraftVanila::assetDownloadFinish() {
	this->assets->deleteLater();
	this->librariesDownloadStart();
}

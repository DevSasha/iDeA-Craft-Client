#include "player.h"

QString Player::nickname = "";
QString Player::sessionToken = "";

Player::Player() {

}

QString Player::getNickname() {
	return nickname;
}

void Player::setNickname(const QString &value) {
	if (nickname != "") return;
	nickname = value;
}

QString Player::getSessionToken() {
	return sessionToken;
}

void Player::setSessionToken(const QString &value) {
	if (sessionToken != "") return;
	sessionToken = value;
}

#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player {
public:
	Player();

	static QString getNickname();
	static void setNickname(const QString &value);

	static QString getSessionToken();
	static void setSessionToken(const QString &value);

private:
	static QString nickname;
	static QString sessionToken;
};

#endif // PLAYER_H

#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>

class APIRequest : public QObject {
	Q_OBJECT
public:
	explicit APIRequest(QString mathod);

signals:

public slots:

private:
	static QNetworkAccessManager *mng;

};

#endif // APIREQUEST_H

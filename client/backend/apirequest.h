#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QObject>

class APIRequest : public QObject {
	Q_OBJECT
public:
	explicit APIRequest(QObject *parent = nullptr);

signals:

public slots:

private:

};

#endif // APIREQUEST_H

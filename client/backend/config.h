#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QSettings>
#include <QVariant>

class Config : public QObject
{
    Q_OBJECT
public:
	static Config *config();
    void save(const QString &key, const QVariant &value);
    QVariant get(const QString &key, const QVariant &defaultValue = QVariant(), bool saveDefault = true);
	bool keyExists(QString key);

signals:

public slots:

private:
	explicit Config();
	QVariant find(const QString &key);

    static Config *pConfig;
    QSettings *set;
};

#endif // CONFIG_H

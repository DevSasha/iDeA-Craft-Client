#include "config.h"

Config *Config::pConfig = nullptr;

Config* Config::config() {
    if (pConfig == nullptr) {
        pConfig = new Config;
    }
    return pConfig;
}

void Config::save(const QString &key, const QVariant &value) {
    QStringList path = key.split('.');

	for (int i = 0; i < path.size() - 1; ++i) {
		set->beginGroup(path[i]);
	}
    set->setValue(path.last(), value);
	for (int i = 0; i < path.size() - 1; ++i) {
		set->endGroup();
	}
}

QVariant Config::get(const QString &key, const QVariant &defaultValue, bool saveDefault) {
	QVariant val = this->find(key);
	if (!val.isNull()) {
		return val;
    } else {
        if (saveDefault)
            this->save(key, defaultValue);
        return defaultValue;
	}
}

bool Config::keyExists(QString key) {
	QVariant val = this->find(key);
	return !val.isNull();
}

Config::Config() {
	this->set = new QSettings(QSettings::UserScope, "idea-craft");
}

QVariant Config::find(const QString &key) {
	QStringList path = key.split('.');
	for (int i = 0; i < path.size() - 1; ++i) {
		set->beginGroup(path[i]);
	}

	QVariant ret = set->value(path.last());
	for (int i = 0; i < path.size() - 1; ++i) {
		set->endGroup();
	}

	return ret;
}

#include "config.h"

Config *Config::pConfig = nullptr;

Config* Config::config() {
    if (pConfig == nullptr) {
        pConfig = new Config;
    }
    return pConfig;
}

void Config::save(const QString &key, const QVariant &value) {
    bool inGroup = false;
    QStringList path = key.split('.');
    if (path.size() > 1) {
        inGroup = true;
        set->beginGroup(path.first());
    }
    set->setValue(path.last(), value);
    if (inGroup)
        set->endGroup();
}

QVariant Config::get(const QString &key, const QVariant &defaultValue, bool saveDefault) {
    if (!set->value(key).isNull()) {
        return set->value("login");
    } else {
        if (saveDefault)
            this->save(key, defaultValue);
        return defaultValue;
    }
}

Config::Config(QObject *parent) : QObject(parent) {
	this->set = new QSettings("idea-craft");
}

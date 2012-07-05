#include "settingsManager.h"

#include <QtCore/QHash>
#include <QtCore/QStringList>

using namespace qReal;

SettingsManager* SettingsManager::mInstance = NULL;

SettingsManager::SettingsManager()
		: mSettings("SPbSU", "QReal")
{
	initDefaultValues();
	load();
}

QVariant SettingsManager::value(QString const &key, QVariant defaultValue)
{
	return instance()->get(key, defaultValue);
}

void SettingsManager::setValue(QString const &name, QVariant const &value)
{
	instance()->set(name, value);
}


SettingsManager* SettingsManager::instance()
{
	if (mInstance == NULL) {
		mInstance = new SettingsManager();
	}
	return mInstance;
}

void SettingsManager::set(QString const &name, QVariant const &value)
{
	mData[name] = value;
}

QVariant SettingsManager::get(QString const &name, QVariant const &defaultValue) const
{
	if (mData.contains(name)) {
		return mData[name];
	}
	return defaultValue;
}

void SettingsManager::saveData()
{
	foreach (QString const &name, mData.keys()) {
		mSettings.setValue(name, mData[name]);
	}
	mSettings.sync();
}

void SettingsManager::load()
{
	foreach (QString const &name, mSettings.allKeys()) {
		mData[name] = mSettings.value(name);
	}
}

void SettingsManager::initDefaultValues()
{
	QSettings values(":/settingsDefaultValues", QSettings::NativeFormat);

	foreach (QString key, values.allKeys()) {
		mDefaultValues.insert(key, values.value(key));
	}
}

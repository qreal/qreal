#include "settingsManager.h"

#include <QtCore/QHash>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>

using namespace qReal;

SettingsManager* SettingsManager::mInstance = NULL;

SettingsManager::SettingsManager()
		: mSettings("SPbSU", "QReal")
{
	initDefaultValues();
	load();
}

void SettingsManager::setValue(QString const &name, QVariant const &value)
{
	instance()->set(name, value);
}

QVariant SettingsManager::value(QString const &key)
{
	return instance()->get(key);
}

QVariant SettingsManager::value(QString const &key, QVariant const &defaultValue)
{
	return instance()->get(key, defaultValue);
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
	if (mDefaultValues.contains(name) && defaultValue == QVariant()) {
		return mDefaultValues[name];
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
	QSettings values(":/settingsDefaultValues", QSettings::IniFormat);

	foreach (QString key, values.allKeys()) {
		mDefaultValues.insert(key, values.value(key));
	}
}

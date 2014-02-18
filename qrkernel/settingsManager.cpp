#include "settingsManager.h"

#include <QtCore/QHash>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>

using namespace qReal;

SettingsManager* SettingsManager::mInstance = nullptr;

SettingsManager::SettingsManager()
	: mSettings("SPbSU", "QReal")
	, mUXInfoInterface(NULL)
{
	initDefaultValues();
	load();
}

void SettingsManager::setValue(QString const &name, QVariant const &value)
{
	instance()->reportValueSetting(name, instance()->value(name), value);
	instance()->set(name, value);
}

void SettingsManager::setUXInfo(UXInfoInterface *uxInfo)
{
	instance()->setUXInfoInterface(uxInfo);
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
	if (mInstance == nullptr) {
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

void SettingsManager::setUXInfoInterface(UXInfoInterface *uxInfo)
{
	mUXInfoInterface = uxInfo;
}

void SettingsManager::reportValueSetting(QString const &name, QVariant const &oldValue, QVariant const &newValue)
{
	if (oldValue == newValue) {
		return;
	}

	if (mUXInfoInterface) {
		mUXInfoInterface->reportSettingsChanges(name, oldValue, newValue);
	}
}

void SettingsManager::saveData()
{
	foreach (QString const &name, mData.keys()) {
		mSettings.setValue(name, mData[name]);
	}
	mSettings.sync();
}

void SettingsManager::saveSettings(QString fileNameForExport)
{
	QSettings settingsForSave(fileNameForExport,QSettings::IniFormat);
	foreach (QString const &name, mData.keys()) {
		settingsForSave.setValue(name, mData[name]);
	}
	settingsForSave.sync();
}

void SettingsManager::load()
{
	foreach (QString const &name, mSettings.allKeys()) {
		mData[name] = mSettings.value(name);
	}
}

void SettingsManager::loadSettings(QString  const &fileNameForImport)
{
	QSettings settings(fileNameForImport,QSettings::IniFormat);
	foreach (QString const &name, settings.allKeys()) {
		mData[name] = settings.value(name);
	}
	saveData();
}

void SettingsManager::initDefaultValues()
{
	QSettings values(":/settingsDefaultValues", QSettings::IniFormat);

	foreach (QString key, values.allKeys()) {
		mDefaultValues.insert(key, values.value(key));
	}
}

void SettingsManager::clearSettings()
{
	instance()->mSettings.clear();
	instance()->mData.clear();
	instance()->mDefaultValues.clear();
}

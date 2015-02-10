#include "settingsManager.h"

#include <QtCore/QHash>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>

using namespace qReal;

SettingsManager* SettingsManager::mInstance = nullptr;

SettingsManager::SettingsManager()
	: mSettings("SPbSU", "QReal")
{
	initDefaultValues();
	load();
}

SettingsManager::~SettingsManager()
{
}

void SettingsManager::setValue(const QString &name, const QVariant &value)
{
	const QVariant oldValue = instance()->value(name);
	if (oldValue != value) {
		instance()->set(name, value);
		emit instance()->settingsChanged(name, oldValue, value);
	}
}

QVariant SettingsManager::value(const QString &key)
{
	return instance()->get(key);
}

QVariant SettingsManager::value(const QString &key, const QVariant &defaultValue)
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

void SettingsManager::set(const QString &name, const QVariant &value)
{
	mData[name] = value;
}

QVariant SettingsManager::get(const QString &name, const QVariant &defaultValue) const
{
	if (mData.contains(name)) {
		return mData[name];
	}

	if (mDefaultValues.contains(name)) {
		return mDefaultValues[name];
	}

	return defaultValue;
}

void SettingsManager::saveData()
{
	for (const QString &name : mData.keys()) {
		mSettings.setValue(name, mData[name]);
	}

	mSettings.sync();
}

void SettingsManager::saveSettings(const QString &fileNameForExport)
{
	QSettings settingsForSave(fileNameForExport, QSettings::IniFormat);
	for (const QString &name : mData.keys()) {
		settingsForSave.setValue(name, mData[name]);
	}

	settingsForSave.sync();
}

void SettingsManager::load()
{
	for (const QString &name : mSettings.allKeys()) {
		mData[name] = mSettings.value(name);
	}
}

void SettingsManager::loadSettings(const QString &fileNameForImport)
{
	mergeSettings(fileNameForImport, mData);
	saveData();
}

void SettingsManager::initDefaultValues()
{
	mergeSettings(":/settingsDefaultValues", mDefaultValues);
}

void SettingsManager::loadDefaultSettings(const QString &filePath)
{
	instance()->mergeSettings(filePath, instance()->mDefaultValues);
}

void SettingsManager::mergeSettings(const QString &fileNameForImport, QHash<QString, QVariant> &target)
{
	QSettings settings(fileNameForImport, QSettings::IniFormat);
	for (const QString &name : settings.allKeys()) {
		target[name] = settings.value(name);
	}
}

void SettingsManager::clearSettings()
{
	instance()->mSettings.clear();
	instance()->mData.clear();
	instance()->mDefaultValues.clear();
}

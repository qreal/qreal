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

void SettingsManager::setValue(QString const &name, QVariant const &value)
{
	QVariant const oldValue = instance()->value(name);
	if (oldValue != value) {
		instance()->set(name, value);
		emit instance()->settingsChanged(name, oldValue, value);
	}
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

void SettingsManager::saveData()
{
	for (QString const &name : mData.keys()) {
		mSettings.setValue(name, mData[name]);
	}

	mSettings.sync();
}

void SettingsManager::saveSettings(QString const &fileNameForExport)
{
	QSettings settingsForSave(fileNameForExport, QSettings::IniFormat);
	for (QString const &name : mData.keys()) {
		settingsForSave.setValue(name, mData[name]);
	}

	settingsForSave.sync();
}

void SettingsManager::load()
{
	for (QString const &name : mSettings.allKeys()) {
		mData[name] = mSettings.value(name);
	}
}

void SettingsManager::loadSettings(QString const &fileNameForImport)
{
	mergeSettings(fileNameForImport, mData);
	saveData();
}

void SettingsManager::initDefaultValues()
{
	mergeSettings(":/settingsDefaultValues", mDefaultValues);
}

void SettingsManager::loadDefaultSettings(QString const &filePath)
{
	instance()->mergeSettings(filePath, instance()->mDefaultValues);
}

void SettingsManager::mergeSettings(QString const &fileNameForImport, QHash<QString, QVariant> &target)
{
	QSettings settings(fileNameForImport, QSettings::IniFormat);
	for (QString const &name : settings.allKeys()) {
		target[name] = settings.value(name);
	}
}

void SettingsManager::clearSettings()
{
	instance()->mSettings.clear();
	instance()->mData.clear();
	instance()->mDefaultValues.clear();
}

QString SettingsManager::convertToString()
{
	QString allSettings;
	QStringList keyList = mSettings.allKeys();
	for (int i = 0; i < keyList.size(); i++)
		allSettings += keyList[i] + "###" + value(keyList[i]).toString() + "#*#";

	return allSettings;
}

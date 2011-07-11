#include "settingsManager.h"
#include <QHash>
#include <QStringList>


SettingsManager* SettingsManager::mInstance = NULL;

SettingsManager::SettingsManager() : mSettings("SPbSU", "QReal")
{
    load();
}

void SettingsManager:: setValue(QString name, QVariant value)
{
    instance()->set(name, value);
}

QVariant SettingsManager::value(QString key, QVariant defaultValue)
{
    return instance()->get(key, defaultValue);
}


SettingsManager* SettingsManager::instance()
{
    if (mInstance == NULL)
	mInstance = new SettingsManager();
    return mInstance;
}

void SettingsManager::set(QString name, QVariant value)
{
    mData[name] = value;
}

QVariant SettingsManager::get(const QString &name, const QVariant &defaultValue) const
{
    if (mData.contains(name))
	return mData[name];
    return defaultValue;
}

void SettingsManager::saveData()
{
    foreach (QString name, mData.keys())
    {
       mSettings.setValue(name, mData[name]);
    }
    mSettings.sync();
}

void SettingsManager::load()
{
    foreach (QString name, mSettings.allKeys())
    {
       mData[name] = mSettings.value(name);
    }
}

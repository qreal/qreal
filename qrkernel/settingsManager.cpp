#include "settingsManager.h"
#include <QHash>
#include <QStringList>

#include <QPoint>
#include <QSize>

SettingsManager* SettingsManager::mInstance = NULL;

SettingsManager::SettingsManager() : mSettings("SPbSU", "QReal")
{
    initDefaultValues();
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

QVariant SettingsManager::value(QString key)
{
    return instance()->get(key);
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
    if (mDefaultValues.contains(name) && defaultValue == QVariant())
        return mDefaultValues[name];
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

void SettingsManager::initDefaultValues()
{
    mDefaultValues.insert("ActivateGrid", true);
    mDefaultValues.insert("ActivateAlignment", true);
    mDefaultValues.insert("Antialiasing", true);
    mDefaultValues.insert("arrangeLinks", true);
    mDefaultValues.insert("autoSave", true);
    mDefaultValues.insert("autoSaveInterval", 600);
    mDefaultValues.insert("buildedFileName", "builded");
    mDefaultValues.insert("builderPath", "gcc");
    mDefaultValues.insert("ChaoticEdition", false);
    mDefaultValues.insert("codeFileName", "code.c");
    mDefaultValues.insert("currentPreferencesTab", 0);
    mDefaultValues.insert("debuggerPath", "gdb");
    mDefaultValues.insert("debuggerTimeout", 750);
    mDefaultValues.insert("debugWorkingDirectory", "");
    mDefaultValues.insert("diagramCreateSuggestion", true);
    mDefaultValues.insert("EmbeddedControls", false);
    mDefaultValues.insert("EmbeddedLinkers", true);
    mDefaultValues.insert("EmbeddedLinkerSize", 6);
    mDefaultValues.insert("Gestures", true);
    mDefaultValues.insert("GridWidth", 10);
    mDefaultValues.insert("gestureDelay", 1000);
    mDefaultValues.insert("IndexGrid", 50);
    mDefaultValues.insert("linuxButton", false);
    mDefaultValues.insert("maximized", true);
    mDefaultValues.insert("OpenGL", true);
    mDefaultValues.insert("otherButton", false);
    mDefaultValues.insert("PaletteIconsInARowCount", 3);
    mDefaultValues.insert("PaletteRepresentation", 0);
    mDefaultValues.insert("PaletteTabSwitching", true);
    mDefaultValues.insert("pathToQmake", "");
    mDefaultValues.insert("pathToMake", "");
    mDefaultValues.insert("pluginExtension", "");
    mDefaultValues.insert("pos", QPoint(0, 0));
    mDefaultValues.insert("prefix", "");
    mDefaultValues.insert("ShowAlignment", true);
    mDefaultValues.insert("ShowGrid", true);
    mDefaultValues.insert("Splashscreen", true);
    mDefaultValues.insert("SquareLine", false);
    mDefaultValues.insert("size", QSize(1024, 800));
    mDefaultValues.insert("temp", "");
    mDefaultValues.insert("warningWindow", true);
    mDefaultValues.insert("windowsButton", false);
    mDefaultValues.insert("workingDir", "./save");
    mDefaultValues.insert("zoomFactor", 2);
}

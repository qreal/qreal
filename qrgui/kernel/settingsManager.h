#pragma once
#include <QtCore/QSettings>
#include <QString>
#include <QHash>
#include <QVariant>
#include <QMutex>

/// @class MyClass - class, that allows to change settings in run-time (replaces QSettings)

class SettingsManager
{
public:
    static SettingsManager* instance();
    static void setValue(QString name, QVariant value);
    static QVariant value(QString key, QVariant defaultValue);
    void saveData();
    void load();

private:

    SettingsManager();
    void set(QString name, QVariant value);
    QVariant get(const QString &key, const QVariant &defaultValue = QVariant()) const;

    static SettingsManager* mInstance;
    QString mDirectory;
    QString mName;
    QHash<QString, QVariant> mData;
    QSettings mSettings;

};



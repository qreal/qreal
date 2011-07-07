#pragma once
#include <QtCore/QSettings>
#include <QString>
#include <QHash>
#include <QVariant>
#include <QMutex>

class SettingsManager
{
public:
    static SettingsManager* instance();
    void setValue(QString name, QVariant value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
    void saveData();
    void load();

private:
    static SettingsManager* mInstance;
    SettingsManager();
    QString mDirectory;
    QString mName;
    QHash<QString, QVariant> mData;
    QSettings mSettings;

};



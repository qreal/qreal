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
    static void setValue(QString name, QVariant value);
    static QVariant value(QString key, QVariant defaultValue);
    void saveData();
    void load();

private:
    //doxygen
    static SettingsManager* mInstance;
    SettingsManager();
    QString mDirectory;
    QString mName;
    QHash<QString, QVariant> mData;
    QSettings mSettings;
    void set(QString name, QVariant value);
    QVariant get(const QString &key, const QVariant &defaultValue = QVariant()) const;

};



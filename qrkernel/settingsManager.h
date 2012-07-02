#pragma once

#include <QtCore/QSettings>
#include <QString>
#include <QHash>
#include <QVariant>
#include <QMutex>

#include "kernelDeclSpec.h"


/// @class SettingsManager - class, that allows to change settings in run-time (replaces QSettings)

class QRKERNEL_EXPORT SettingsManager
{
public:

    static QVariant value(QString key);
    static QVariant value(QString key, QVariant defaultValue);
    static void setValue(QString name, QVariant value = QVariant());
    void saveData();
    void load();

    static SettingsManager* instance();

private:
    void set(QString name, QVariant value);
    QVariant get(const QString &key, const QVariant &defaultValue = QVariant()) const;

    void initDefaultValues();

    static SettingsManager* mInstance;
    SettingsManager();
    QString mDirectory;
    QString mName;
    QHash<QString, QVariant> mData;
    QHash<QString, QVariant> mDefaultValues;
    QSettings mSettings;

};



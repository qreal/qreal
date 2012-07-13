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
	static QVariant value(QString const &key);
	static QVariant value(QString const &key, QVariant const &defaultValue);
	static void setValue(QString const &name, QVariant const &value = QVariant());
	void saveData();
	void load();

	static SettingsManager* instance();

private:
	void set(QString const &name, QVariant const &value);
	QVariant get(QString const &key, QVariant const &defaultValue = QVariant()) const;

	void initDefaultValues();

	static SettingsManager* mInstance;
	SettingsManager();
	QString mDirectory;
	QString mName;
	QHash<QString, QVariant> mData;
	QHash<QString, QVariant> mDefaultValues;
	QSettings mSettings;

};

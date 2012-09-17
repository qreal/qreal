#pragma once

#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QVariant>

#include "kernelDeclSpec.h"

namespace qReal {

/// Singleton class that allows to change settings in run-time
/// (replaces QSettings). Purpose of this class is to allow two instances
/// of an application coexist without changing each other's settings,
/// by storing settings separately in memory for each instance and syncing
/// them only on start/exit.
class QRKERNEL_EXPORT SettingsManager
{
public:
	SettingsManager();
	static QVariant value(QString const &key);
	static QVariant value(QString const &key, QVariant const &defaultValue);
	static void setValue(QString const &name, QVariant const &value = QVariant());
	void saveData();

	/// Loads settings from persistent external storage into SettingsManager.
	void load();

	static SettingsManager* instance();

private:
	void set(QString const &name, QVariant const &value);
	QVariant get(QString const &key, QVariant const &defaultValue = QVariant()) const;

	void initDefaultValues();

	/// Singleton sole instance.
	static SettingsManager* mInstance;

	/// In-memory settings storage.
	QHash<QString, QVariant> mData;
	QHash<QString, QVariant> mDefaultValues;
	QSettings mSettings;
};

}

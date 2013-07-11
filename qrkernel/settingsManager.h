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
	/// Get value associated with given key from settings.
	/// @param key Parameter name.
	/// @returns Variant with parameter value.
	static QVariant value(QString const &key);

	/// Get value associated with given key from settings with given default value.
	/// @param key Parameter name.
	/// @param defaultValue Default value, used when parameter not found.
	/// @returns Variant with parameter value.
	static QVariant value(QString const &key, QVariant const &defaultValue);

	/// Set value associated with given key.
	/// @param key Parameter name.
	/// @param value Parameter value.
	static void setValue(QString const &key, QVariant const &value);

	/// Removes all entries in persistent external storage
	static void clearSettings();

	/// Returns an instance of a singleton.
	static SettingsManager* instance();

	/// Saves settings into persistent external storage (for example, Windows
	/// registry), making them available to new instances of an application.
	void saveData();

	/// Loads settings from persistent external storage into SettingsManager.
	void load();

private:
	/// Private constructor.
	SettingsManager();
	void set(QString const &name, QVariant const &value);
	QVariant get(QString const &key, QVariant const &defaultValue = QVariant()) const;

	void initDefaultValues();

	//QMap<QString, QVariant> mDefaultValues;

	/// Singleton sole instance.
	static SettingsManager* mInstance;

	/// In-memory settings storage.
	QHash<QString, QVariant> mData;
	QHash<QString, QVariant> mDefaultValues;
	/// Persistent settings storage.
	QSettings mSettings;
};

}

#pragma once

#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QVariant>

#include "kernelDeclSpec.h"
#include "uxInfoInterface.h"

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

	/// Set UXInfo for saving settings changers.
	/// @param uxInfo UXInfo interface
	static void setUXInfo(UXInfoInterface *uxInfo);

	/// Removes all entries in persistent external storage
	static void clearSettings();

	/// Returns an instance of a singleton.
	static SettingsManager *instance();

	/// Saves settings into persistent external storage (for example, Windows
	/// registry), making them available to new instances of an application.
	void saveData();

	/// Saves settings into selected file with name fileNameForExport.
	void saveSettings(QString fileNameForExport);

	/// Loads settings from persistent external storage into SettingsManager.
	void load();

	/// Loads settings from selected file with name fileNameForImport.
	void loadSettings(const QString &fileNameForImport);

private:
	/// Private constructor.
	SettingsManager();
	~SettingsManager();

	void set(QString const &name, QVariant const &value);
	QVariant get(QString const &key, QVariant const &defaultValue = QVariant()) const;
	/// Initialization of UXInfoInterface.
	void setUXInfoInterface(UXInfoInterface *uxInfo);
	void reportValueSetting(QString const &name, QVariant const &oldValue, QVariant const &newValue);

	void initDefaultValues();

	/// Singleton sole instance.
	static SettingsManager* mInstance;

	/// In-memory settings storage.
	QHash<QString, QVariant> mData;
	QHash<QString, QVariant> mDefaultValues;
	/// Persistent settings storage.
	QSettings mSettings;
	UXInfoInterface* mUXInfoInterface; // Has ownership
};

}

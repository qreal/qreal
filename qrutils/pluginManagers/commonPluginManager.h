#pragma once

#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QObject>
#include <QtCore/QMap>

#include "qrutils/utilsDeclSpec.h"

namespace qReal {

/// Common part of plugin loading.
class QRUTILS_EXPORT CommonPluginManager
{
public:
	/// @param applicationDirPath - path to qrgui.exe
	/// @param additionalPart - path to folder with plugins, usually "bin/plugins", for robots can be folder inside plugins folder
	CommonPluginManager(
			QString const &applicationDirPath
			, QString const &additionalPart = "plugins"
			);
	~CommonPluginManager();

	/// Deletes all loaders.
	void deleteAllLoaders();

	/// Returns list of all found plugins if succeed and empty list otherwise.
	QList<QObject *> loadAllPlugins();

	/// Returns plugin found by name if succeed and nothing otherwise
	/// and error message, if failed.
	/// @param pluginName - name of plugin to load
	/// @returns loaded plugin and error message
	QPair<QObject *, QString> pluginLoadedByName(QString const &pluginName);

	/// Unloads plugins, given filename
	/// @param pluginName - name of plugin
	/// @returns error message if there was error and empty string otherwise
	QString unloadPlugin(QString const &pluginName);

	/// Returns fileName by given object.
	QString fileName(QObject *plugin) const;

private:
	/// Directory to loaded plugins.
	QDir mPluginsDir;

	/// Map from name to loader
	/// Has ownersip.
	QMap<QString, QPluginLoader *> mLoaders;

	/// Map from fileName to plugin
	/// Has ownership.
	QMap<QString, QObject *> mNameAndObject;

	/// Path to application directory, used to count path to loaded plugins.
	QString mApplicationDirectoryPath;

	///Additional part of path.
	/// "plugins" if we're trying to load plugins from "bin/plugins"
	/// for robots it can be "plugins/kitPlugins", for example
	QString mAdditionalPart;
};
}

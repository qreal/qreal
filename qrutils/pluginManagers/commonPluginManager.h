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
	QList<QObject *> allLoadedPlugins();

	/// Returns plugin found by name if succeed and nothing otherwise
	/// and error message, if failed.
	QPair<QObject *, QString> pluginLoadedByName(QString const &pluginName);

	/// Unloads plugins, given filename
	/// first member in pair is error message
	/// second member is true if plugin was unloaded
	/// third member is true if loader was found
	/// @param pluginName - name of plugin
	QPair<QString, QPair<bool, bool> > unloadPlugin(QString const &pluginName);

	/// Returns fileName by given object.
	QString fileName(QObject *plugin) const;

private:
	/// Directory to loaded plugins.
	QDir mPluginsDir;
	/// Map from name to loader.
	QMap<QString, QPluginLoader *> mLoaders;
	/// Map from fileName to plugin.
	QMap<QString, QObject *> mNameAndObject;
	/// Path to application directory, used to count path to loaded plugins.
	QString mApplicationDirectoryPath;
	///Additional part of path.
	/// "plugins" if we're trying to load plugins from "bin/plugins"
	/// for robots it can be "plugins/kitPlugins", for example
	QString mAdditionalPart;
};
}

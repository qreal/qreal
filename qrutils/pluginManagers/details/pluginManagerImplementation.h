/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QObject>
#include <QtCore/QMap>

#include "qrutils/utilsDeclSpec.h"

namespace qReal {
namespace details {

/// Common part of plugin loading.
class QRUTILS_EXPORT PluginManagerImplementation
{
public:
	/// @param applicationDirPath - path to qrgui.exe
	/// @param additionalPart - path to folder with plugins, usually "bin/plugins", for robots can be folder inside
	///        plugins folder
	PluginManagerImplementation(
			const QString &applicationDirPath
			, const QString &additionalPart = "plugins"
			);

	~PluginManagerImplementation();

	/// Returns list of all found plugins if succeed and empty list otherwise.
	QList<QObject *> loadAllPlugins();

	/// Returns plugin found by name if succeed and nothing otherwise
	/// and error message, if failed.
	/// @param pluginName - name of plugin to load
	/// @returns loaded plugin and error message
	QPair<QObject *, QString> pluginLoadedByName(const QString &pluginName);

	/// Unloads plugins, given filename
	/// @param pluginName - name of plugin
	/// @returns error message if there was error and empty string otherwise
	QString unloadPlugin(const QString &pluginName);

	/// Returns fileName by given object.
	QString fileName(QObject *plugin) const;

private:
	/// Directory to loaded plugins.
	QDir mPluginsDir;

	/// Map from name to loader
	/// Has ownership.
	QMap<QString, QPluginLoader *> mLoaders;

	/// Map from fileName to plugin
	/// Has ownership.
	QMap<QString, QObject *> mFileNameAndPlugin;

	/// Path to application directory, used to count path to loaded plugins.
	QString mApplicationDirectoryPath;

	///Additional part of path.
	/// "plugins" if we're trying to load plugins from "bin/plugins"
	/// for robots it can be "plugins/kitPlugins", for example
	QString mAdditionalPart;
};

}
}

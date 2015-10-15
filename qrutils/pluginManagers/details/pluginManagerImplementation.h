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
	/// @param pluginsDirPath - path to directory containing plugins to be loaded.
	explicit PluginManagerImplementation(const QString &pluginsDirPath);
	~PluginManagerImplementation();

	/// Returns list of all found plugins if succeed and empty list otherwise.
	QList<QObject *> loadAllPlugins();

	/// Returns plugin found by name if succeed and nothing otherwise
	/// and error message, if failed.
	/// @param pluginName - name of plugin to load
	/// @returns loaded plugin and error message
	QPair<QObject *, QString> loadPluginByName(const QString &pluginName);

	/// Unloads plugins, given filename
	/// @param pluginName - name of plugin
	/// @returns error message if there was error and empty string otherwise
	QString unloadPlugin(const QString &pluginName);

	/// Returns fileName by given object.
	QString fileName(QObject *plugin) const;

	/// Returns loaded plugin object by name specified in plugin metainformation.
	QObject *pluginByName(const QString &pluginName) const;

	/// Returns names of all plugins.
	QList<QString> namesOfPlugins() const;

private:
	/// Directory containing plugins to be loaded.
	QDir mPluginsDir;

	/// List of loaders with names of plugins.
	QList<QPair<QString, QPluginLoader *>> mLoaders;

	/// Map from fileName to plugin
	/// Has ownership.
	QMap<QString, QObject *> mFileNameAndPlugin;
};

}
}

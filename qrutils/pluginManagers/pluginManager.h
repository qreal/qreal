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

#include <QtCore/QObject>

#include "details/pluginManagerImplementation.h"

#include "qrutils/utilsDeclSpec.h"

namespace qReal {

/// Template class that allows to load/unload plugins and return their corresponding interfaces.
class QRUTILS_EXPORT PluginManager
{
public:
	explicit PluginManager(const QString &pluginsDirPath);

	/// Returns list of all found plugins if succeed and empty list otherwise.
	template <class InterfaceType>
	QList<InterfaceType *> loadAllPlugins()
	{
		QList<QObject *> const loadedPlugins = mPluginManagerLoader.loadAllPlugins();
		return listOfInterfaces<InterfaceType>(loadedPlugins);
	}

	/// Returns plugin found by name if succeed and nothing otherwise
	/// and error message, if failed.
	/// @param pluginName - name of plugin to load
	/// @returns loaded plugin and error message
	template <class InterfaceType>
	QPair<InterfaceType *, QString> pluginLoadedByName(const QString &pluginName)
	{
		QPair<QObject *, QString> resultOfLoading = mPluginManagerLoader.loadPluginByName(pluginName);

		const QObject *loadedPlugin = resultOfLoading.first;
		const QString errorMessage = resultOfLoading.second;

		return qMakePair(qobject_cast<InterfaceType *>(loadedPlugin), errorMessage);
	}

	/// Unloads plugins, given filename
	/// @param pluginName - name of plugin
	/// @returns error message if there was error and empty string otherwise
	QString unloadPlugin(const QString &pluginName);

	/// Returns names of all plugins.
	QList<QString> namesOfPlugins() const;

	/// Returns fileName by given object.
	template <class InterfaceType>
	QString fileName(InterfaceType *plugin) const
	{
		return mPluginManagerLoader.fileName(reinterpret_cast<QObject *>(plugin));
	}

	/// Returns plugin object instance by the name specified plugin metainformation.
	/// The plugin must be loaded and initialized, otherwise nullptr will be returned.
	template <class InterfaceType>
	InterfaceType *plugin(const QString &pluginName) const
	{
		return dynamic_cast<InterfaceType *>(mPluginManagerLoader.pluginByName(pluginName));
	}

private:
	/// Casts list of objects to list of interfaces.
	template <class InterfaceType>
	QList<InterfaceType *> listOfInterfaces(QList<QObject *> const interfacesToWrap) const
	{
		QList<InterfaceType *> interfacesList;

		for (const QObject *currentInterface : interfacesToWrap) {
			InterfaceType *castedInterface = qobject_cast<InterfaceType *>(currentInterface);
			if (castedInterface) {
				interfacesList.append(castedInterface);
			}
		}

		return interfacesList;
	}

	/// Implementation of plugin loading methods.
	details::PluginManagerImplementation mPluginManagerLoader;
};
}

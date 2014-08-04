#pragma once

#include <QtCore/QObject>

#include "details/pluginManagerImplementation.h"

#include "qrutils/utilsDeclSpec.h"

namespace qReal {

/// Template class that allows to load/unload plugins and return their corresponding interfaces.
class QRUTILS_EXPORT PluginManager
{
public:
	PluginManager(QString const &applicationDirPath, QString const &additionalPart);

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
	QPair<InterfaceType *, QString> pluginLoadedByName(QString const &pluginName)
	{
		QPair<QObject *, QString> resultOfLoading = mPluginManagerLoader.pluginLoadedByName(pluginName);

		QObject const *loadedPlugin = resultOfLoading.first;
		QString const errorMessage = resultOfLoading.second;

		return qMakePair(qobject_cast<InterfaceType *>(loadedPlugin), errorMessage);
	}

	/// Unloads plugins, given filename
	/// @param pluginName - name of plugin
	/// @returns error message if there was error and empty string otherwise
	QString unloadPlugin(QString const &pluginName);

	/// Returns fileName by given object.
	template <class InterfaceType>
	QString fileName(InterfaceType *plugin) const
	{
		return mPluginManagerLoader.fileName(reinterpret_cast<QObject *>(plugin));
	}

private:
	/// Casts list of objects to list of interfaces.
	template <class InterfaceType>
	QList<InterfaceType *> listOfInterfaces(QList<QObject *> const interfacesToWrap) const
	{
		QList<InterfaceType *> interfacesList;

		for (QObject const *currentInterface : interfacesToWrap) {
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

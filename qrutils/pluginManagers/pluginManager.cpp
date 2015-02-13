#include "pluginManager.h"

using namespace qReal;

PluginManager::PluginManager(
		const QString &applicationDirPath
		, const QString &additionalPart)
	: mPluginManagerLoader(details::PluginManagerImplementation(applicationDirPath, additionalPart))
{
}

QString PluginManager::unloadPlugin(const QString &pluginName)
{
	return mPluginManagerLoader.unloadPlugin(pluginName);
}

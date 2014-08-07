#include "pluginManager.h"

using namespace qReal;

PluginManager::PluginManager(
		QString const &applicationDirPath
		, QString const &additionalPart)
	: mPluginManagerLoader(details::PluginManagerImplementation(applicationDirPath, additionalPart))
{
}

QString PluginManager::unloadPlugin(QString const &pluginName)
{
	return mPluginManagerLoader.unloadPlugin(pluginName);
}

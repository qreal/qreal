#include "interpretedPluginManager.h"

#include <QtWidgets/QApplication>

using namespace qReal;

InterpretedPluginManager::InterpretedPluginManager()
	: mCustomizer()
	, mPluginManager(PluginManager(qApp->applicationDirPath(), "plugins/interpreted"))
{
	mPlugins = mPluginManager.loadAllPlugins<InterpretedPluginInterface>();
}

InterpretedPluginManager::~InterpretedPluginManager()
{
}

void InterpretedPluginManager::init(const PluginConfigurator &configurator, qrRepo::LogicalRepoApi &metamodelRepoApi)
{
	for (InterpretedPluginInterface * const interpretedPlugin : mPlugins) {
		interpretedPlugin->init(configurator, metamodelRepoApi);
	}
}

QList<ActionInfo> InterpretedPluginManager::actions() const
{
	QList<ActionInfo> result;
	for (InterpretedPluginInterface * const toolPlugin : mPlugins) {
		result += toolPlugin->actions();
	}

	return result;
}

void InterpretedPluginManager::loadDefaultSettings()
{
}

void InterpretedPluginManager::setHotKeyActions() const
{
}

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

void InterpretedPluginManager::init(
		const PluginConfigurator &configurator
		, qrRepo::LogicalRepoApi &metamodelRepoApi
		, EditorManagerInterface *editorManagerInterface)
{
	for (InterpretedPluginInterface * const interpretedPlugin : mPlugins) {
		interpretedPlugin->init(configurator, metamodelRepoApi, editorManagerInterface);
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

QList<QAction *> InterpretedPluginManager::menuActionsList() const
{
	QList<QAction *> result;
	for (InterpretedPluginInterface * const interpretedPlugin : mPlugins) {
		result += interpretedPlugin->menuActionList();
	}

	return result;
}

void InterpretedPluginManager::loadDefaultSettings()
{
}

void InterpretedPluginManager::setHotKeyActions() const
{
}

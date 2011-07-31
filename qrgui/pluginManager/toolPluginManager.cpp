#include "toolPluginManager.h"

#include <QtCore/QPluginLoader>
#include <QtGui/QApplication>

using namespace qReal;

ToolPluginManager::ToolPluginManager(QObject *parent)
{
	mPluginsDir = QDir(qApp->applicationDirPath());

	while (!mPluginsDir.isRoot() && !mPluginsDir.entryList(QDir::Dirs).contains("plugins")) {
		mPluginsDir.cdUp();
	}

	mPluginsDir.cd("plugins");

	foreach (QString fileName, mPluginsDir.entryList(QDir::Files)) {
		// TODO: Free memory
		QPluginLoader *loader  = new QPluginLoader(mPluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader->instance();

		if (plugin) {
			ToolPluginInterface *toolPlugin = qobject_cast<ToolPluginInterface *>(plugin);
			if (toolPlugin) {
				mPlugins << toolPlugin;
			}
		}
	}
}

ToolPluginManager::~ToolPluginManager()
{
//	foreach (ToolPluginInterface *toolPlugin, mPlugins)
//		delete toolPlugin;
}

void ToolPluginManager::init(PluginConfigurator const &configurator)
{
	foreach (ToolPluginInterface *toolPlugin, mPlugins)
		toolPlugin->initPlugin(configurator);
}

QList<CustomToolInterface::ActionInfo> ToolPluginManager::actions() const
{
	QList<CustomToolInterface::ActionInfo> result;
	foreach (ToolPluginInterface *toolPlugin, mPlugins) {
		foreach (CustomToolInterface *customTool, toolPlugin->toolInterfaces())
			foreach (CustomToolInterface::ActionInfo action, customTool->actions())
				result << action;
	}

	return result;
}

QList<QPair<QString, PreferencesPage *> > ToolPluginManager::preferencesPages() const
{
	QList<QPair<QString, PreferencesPage *> > result;
	foreach (ToolPluginInterface *toolPlugin, mPlugins)
		foreach (CustomToolInterface *customTool, toolPlugin->toolInterfaces())
			if (customTool->preferencesPage().second != NULL)
				result << customTool->preferencesPage();

	return result;
}

CustomizationInterface *ToolPluginManager::customizer() const
{
	foreach (ToolPluginInterface *toolPlugin, mPlugins)
		if (toolPlugin->customizationInterface())
			return toolPlugin->customizationInterface();
	return NULL;
}

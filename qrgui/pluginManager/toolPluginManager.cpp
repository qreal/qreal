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

void ToolPluginManager::init(qrRepo::RepoControlInterface *repo)
{
	foreach (ToolPluginInterface *toolPlugin, mPlugins)
		toolPlugin->initPlugin(repo);
}

QList<CustomToolInterface::ActionInfo> ToolPluginManager::actions() const
{
	QList<CustomToolInterface::ActionInfo> result;
	foreach (ToolPluginInterface *toolPlugin, mPlugins) {
		foreach (CustomToolInterface *customTool, toolPlugin->toolInterfaces())
			foreach (CustomToolInterface::ActionInfo action, customTool->actions())
				result << action;
		foreach (InterpreterInterface *interpreter, toolPlugin->interpreterInterfaces())
			foreach (QAction *action, interpreter->customActions())
				result << CustomToolInterface::ActionInfo(action, "interpreters", "tools");
	}

	return result;
}

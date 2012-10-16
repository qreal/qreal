#include <QtGui/QApplication>

#include "toolPluginManager.h"

using namespace qReal;

ToolPluginManager::ToolPluginManager(QObject *parent)
		: mCustomizer()
{
	Q_UNUSED(parent)
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
				mLoaders << loader;
			}
			else {
				// TODO: Does not work on linux. See editorManager.cpp for more details.
				// loader->unload();
				delete loader;
			}
		} else {
			loader->unload();
			delete loader;
		}
	}
}

ToolPluginManager::~ToolPluginManager()
{
	foreach (QPluginLoader *loader, mLoaders) {
		delete loader;
	}
}

void ToolPluginManager::init(PluginConfigurator const &configurator)
{
	foreach (ToolPluginInterface *toolPlugin, mPlugins) {
		toolPlugin->init(configurator);
	}
}

QList<ActionInfo> ToolPluginManager::actions() const
{
	QList<ActionInfo> result;
	foreach (ToolPluginInterface *toolPlugin, mPlugins) {
		result += toolPlugin->actions();
	}

	return result;
}

QList<QPair<QString, PreferencesPage *> > ToolPluginManager::preferencesPages() const
{
	QList<QPair<QString, PreferencesPage *> > result;
	foreach (ToolPluginInterface *toolPlugin, mPlugins) {
		if (toolPlugin->preferencesPage().second != NULL) {
			result << toolPlugin->preferencesPage();
		}
	}

	return result;
}

Customizer const *ToolPluginManager::customizer() const
{
	foreach (ToolPluginInterface *toolPlugin, mPlugins) {
		if (toolPlugin->customizationInterface()) {
			return toolPlugin->customizationInterface();
		}
	}
	return &mCustomizer;
}

void ToolPluginManager::updateSettings()
{
	foreach (ToolPluginInterface *toolPlugin, mPlugins) {
		toolPlugin->updateSettings();
	}
}

void ToolPluginManager::activeTabChanged(Id const & rootElementId)
{
	foreach (ToolPluginInterface *toolPlugin, mPlugins) {
		toolPlugin->activeTabChanged(rootElementId);
	}
}

QList<ToolPluginInterface *> ToolPluginManager::getPlugins()
{
	return mPlugins;
}

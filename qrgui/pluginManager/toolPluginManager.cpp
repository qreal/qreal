#include "toolPluginManager.h"

#include <QtWidgets/QApplication>

#include "hotKeyManager/hotKeyManager.h"

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

	foreach (QString const &fileName, mPluginsDir.entryList(QDir::Files)) {
		// TODO: Free memory
		QPluginLoader *loader = new QPluginLoader(mPluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader->instance();

		if (plugin) {
			ToolPluginInterface *toolPlugin = qobject_cast<ToolPluginInterface *>(plugin);
			if (toolPlugin) {
				mPlugins << toolPlugin;
				mLoaders << loader;
			} else {
				// TODO: Does not work on linux. See editorManager.cpp for more details.
				// loader->unload();
				delete loader;
			}
		} else {
			loader->unload();
			delete loader;
		}
	}

	setHotKeyActions();
}

ToolPluginManager::~ToolPluginManager()
{
	foreach (QPluginLoader *loader, mLoaders) {
		delete loader;
	}
}

void ToolPluginManager::init(PluginConfigurator const &configurator)
{
	mSystemEvents = &configurator.systemEvents();

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

QList<HotKeyActionInfo> ToolPluginManager::hotKeyActions() const
{
	QList<HotKeyActionInfo> result;
	foreach (ToolPluginInterface *toolPlugin, mPlugins) {
		result += toolPlugin->hotKeyActions();
	}

	return result;
}

void ToolPluginManager::setHotKeyActions() const
{
	foreach (HotKeyActionInfo const &actionInfo, hotKeyActions()) {
		HotKeyManager::setCommand(actionInfo.id(), actionInfo.label(), actionInfo.action());
	}
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

Customizer *ToolPluginManager::customizer() const
{
	foreach (ToolPluginInterface *toolPlugin, mPlugins) {
		if (toolPlugin->customizationInterface()) {
			return toolPlugin->customizationInterface();
		}
	}
	return const_cast<qReal::Customizer *>(&mCustomizer);
}

void ToolPluginManager::updateSettings()
{
	mSystemEvents->emitSettingsUpdated();
}

void ToolPluginManager::activeTabChanged(Id const & rootElementId)
{
	mSystemEvents->emitActiveTabChanged(rootElementId);
}

QList<ToolPluginInterface *> ToolPluginManager::getPlugins()
{
	return mPlugins;
}

#include "toolPluginManager.h"

#include <QtWidgets/QApplication>

#include <qrkernel/logging.h>
#include <qrkernel/settingsManager.h>

#include "hotKeyManager/hotKeyManager.h"

using namespace qReal;

ToolPluginManager::ToolPluginManager()
	: mCustomizer()
	, mPluginManager(PluginManager(qApp->applicationDirPath(), "plugins/tools"))
{
	mPlugins = mPluginManager.loadAllPlugins<ToolPluginInterface>();

	loadDefaultSettings();
}

ToolPluginManager::~ToolPluginManager()
{
}

void ToolPluginManager::init(PluginConfigurator const &configurator)
{
	QLOG_INFO() << "Initializing tool plugins...";
	mSystemEvents = &configurator.systemEvents();

	for (ToolPluginInterface * const toolPlugin : mPlugins) {
		toolPlugin->init(configurator);
	}
}

QList<ActionInfo> ToolPluginManager::actions() const
{
	QList<ActionInfo> result;
	for (ToolPluginInterface * const toolPlugin : mPlugins) {
		result += toolPlugin->actions();
	}

	return result;
}

QList<HotKeyActionInfo> ToolPluginManager::hotKeyActions() const
{
	QList<HotKeyActionInfo> result;
	for (ToolPluginInterface * const toolPlugin : mPlugins) {
		result += toolPlugin->hotKeyActions();
	}

	return result;
}

void ToolPluginManager::loadDefaultSettings()
{
	for (ToolPluginInterface * const toolPlugin : mPlugins) {
		for (const QString &defaultSettingsFile : toolPlugin->defaultSettingsFiles()) {
			SettingsManager::loadDefaultSettings(defaultSettingsFile);
		}
	}
}

QList<QPair<QString, gui::PreferencesPage *>> ToolPluginManager::preferencesPages() const
{
	QList<QPair<QString, gui::PreferencesPage *>> result;
	for (ToolPluginInterface * const toolPlugin : mPlugins) {
		if (toolPlugin->preferencesPage().second) {
			result << toolPlugin->preferencesPage();
		}
	}

	return result;
}

QMultiMap<QString, ProjectConverter> ToolPluginManager::projectConverters() const
{
	QMultiMap<QString, ProjectConverter> result;
	for (ToolPluginInterface * const toolPlugin : mPlugins) {
		for (ProjectConverter const &converter : toolPlugin->projectConverters()) {
			result.insertMulti(converter.editor(), converter);
		}
	}

	return result;
}

Customizer *ToolPluginManager::customizer() const
{
	for (ToolPluginInterface * const toolPlugin : mPlugins) {
		if (toolPlugin->customizationInterface()) {
			return toolPlugin->customizationInterface();
		}
	}
	return const_cast<qReal::Customizer *>(&mCustomizer);
}

void ToolPluginManager::updateSettings()
{
	emit mSystemEvents->settingsUpdated();
}

void ToolPluginManager::activeTabChanged(const Id & rootElementId)
{
	emit mSystemEvents->activeTabChanged(rootElementId);
}

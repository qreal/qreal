#include "toolPluginManager.h"

#include <QtWidgets/QApplication>

#include <qrkernel/logging.h>

#include "hotKeyManager/hotKeyManager.h"

using namespace qReal;

ToolPluginManager::ToolPluginManager()
	: mCustomizer()
	, mPluginManager(PluginManager(qApp->applicationDirPath(), "plugins/tools"))
{
	mPlugins = mPluginManager.loadAllPlugins<ToolPluginInterface>();

	loadDefaultSettings();
	setHotKeyActions();
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

void ToolPluginManager::setHotKeyActions() const
{
	for (HotKeyActionInfo const &actionInfo : hotKeyActions()) {
		HotKeyManager::setCommand(actionInfo.id(), actionInfo.label(), actionInfo.action());
	}
}

void ToolPluginManager::loadDefaultSettings()
{
	for (ToolPluginInterface * const toolPlugin : mPlugins) {
		for (QString const &defaultSettingsFile : toolPlugin->defaultSettingsFiles()) {
			SettingsManager::loadDefaultSettings(defaultSettingsFile);
		}
	}
}

QList<QPair<QString, PreferencesPage *>> ToolPluginManager::preferencesPages() const
{
	QList<QPair<QString, PreferencesPage *>> result;
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

QString ToolPluginManager::mainToolPlugin() const
{
	for (ToolPluginInterface * const toolPlugin : mPlugins) {
		if (toolPlugin->customizationInterface()) {
			return dynamic_cast<QObject *>(toolPlugin)->objectName();
		}
	}

	return QString();
}

void ToolPluginManager::updateSettings()
{
	emit mSystemEvents->settingsUpdated();
}

void ToolPluginManager::activeTabChanged(Id const & rootElementId)
{
	emit mSystemEvents->activeTabChanged(rootElementId);
}

/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "toolPluginManager.h"

#include <QtWidgets/QApplication>

#include <qrkernel/logging.h>
#include <qrkernel/settingsManager.h>
#include <qrkernel/platformInfo.h>

#include "hotKeyManager/hotKeyManager.h"

using namespace qReal;

ToolPluginManager::ToolPluginManager()
	: mCustomizer()
	, mPluginManager(PlatformInfo::invariantSettingsPath("pathToToolPlugins"))
{
	mPlugins = mPluginManager.loadAllPlugins<ToolPluginInterface>();

	loadDefaultSettings();
}

ToolPluginManager::~ToolPluginManager()
{
}

void ToolPluginManager::init(const PluginConfigurator &configurator)
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
		for (const ProjectConverter &converter : toolPlugin->projectConverters()) {
			result.insertMulti(converter.editor(), converter);
		}
	}

	return result;
}

const Customizer *ToolPluginManager::customizer() const
{
	for (ToolPluginInterface * const toolPlugin : mPlugins) {
		if (toolPlugin->customizationInterface()) {
			return toolPlugin->customizationInterface();
		}
	}

	return &mCustomizer;
}

void ToolPluginManager::updateSettings()
{
	emit mSystemEvents->settingsUpdated();
}

void ToolPluginManager::activeTabChanged(const TabInfo &info)
{
	emit mSystemEvents->activeTabChanged(info);
}

QObject *ToolPluginManager::pluginGuiFacade(const QString &pluginName) const
{
	return (mPluginManager.plugin<ToolPluginInterface>(pluginName))->guiScriptFacade();
}

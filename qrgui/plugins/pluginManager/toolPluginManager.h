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

#pragma once

#include <QtCore/QDir>
#include <QtCore/QList>
#include <QtCore/QPluginLoader>
#include <QtWidgets/QAction>

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QMap>

#include <qrutils/pluginManagers/pluginManager.h>
#include <qrgui/preferencesDialog/preferencesPage.h>

#include <plugins/toolPluginInterface/toolPluginInterface.h>
#include <plugins/toolPluginInterface/customizer.h>
#include <plugins/toolPluginInterface/pluginConfigurator.h>
#include <plugins/toolPluginInterface/hotKeyActionInfo.h>
#include <plugins/toolPluginInterface/systemEvents.h>

#include "plugins/pluginManager/pluginsManagerDeclSpec.h"

namespace qReal {

class QRGUI_PLUGINS_MANAGER_EXPORT ToolPluginManager
{
public:
	ToolPluginManager();
	~ToolPluginManager();

	void init(const PluginConfigurator &configurator);
	void updateSettings();
	QList<ActionInfo> actions() const;
	QList<QPair<QString, gui::PreferencesPage *> > preferencesPages() const;

	/// Returns a list of hotkeys specified in plugins.
	QList<HotKeyActionInfo> hotKeyActions() const;

	/// Returns a multimap of project conveters to editors whoose diagrams they convert.
	/// @warning The result is obtained each time from scratch when you call this method so better to memorize it.
	QMultiMap<QString, ProjectConverter> projectConverters() const;

	/// Returns GUI customizer object. In each QReal's instance there should be only one plugin with
	/// valid customizationInterface(): robots plugin for QReal:Robots, ubiq plugin for QReal:Ubiq etc.
	/// If there're more than one plugin of that kind, it is not specified which one will be used.
	const Customizer *customizer() const;

	/// Notifies plugins about change of currently open tab. The passed in parameter structure contains information
	/// about newly opened tab (or maybe null info in case of last tab is closed).
	void activeTabChanged(const TabInfo &info);

	/// Returns GUI script facade of the given plugin.
	/// The resulting object provides a convenient way to work with different elements of plugin graphical user
	/// interface in scripting language.
	QObject *pluginGuiFacade(const QString &pluginName) const;

private:
	/// Asks plugins for custom default settings and merges them with engine`s ones.
	void loadDefaultSettings();

	QDir mPluginsDir;

	QList<ToolPluginInterface *> mPlugins;

	/// An object and that is used to customize GUI with values from plugins
	const Customizer mCustomizer;

	/// Common part of all plugin managers
	PluginManager mPluginManager;

	SystemEvents *mSystemEvents;
};

}

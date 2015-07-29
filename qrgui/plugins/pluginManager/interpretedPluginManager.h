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

#include <QtWidgets/QAction>

#include <qrutils/pluginManagers/pluginManager.h>

#include <plugins/interpretedPluginInterface/interpretedPluginInterface.h>
#include "plugins/pluginManager/pluginsManagerDeclSpec.h"

namespace qReal {

/// This class inits interpreter plugins and returns their actions and preferences pages for mainwindow.
class QRGUI_PLUGINS_MANAGER_EXPORT InterpretedPluginManager
{
public:
	InterpretedPluginManager();
	~InterpretedPluginManager();

	/// Inits interpreted plugins.
	/// @param configurator - information about model
	/// @param metamodelRepoApi - information about metamodel
	void init(const PluginConfigurator &configurator, qrRepo::LogicalRepoApi &metamodelRepoApi
			, EditorManagerInterface *editorManagerInterface);

	/// Returns list of actions of all plugins.
	QList<ActionInfo> actions() const;
	/// Returns list of all preferences pages.
	QList<QPair<QString, gui::PreferencesPage *>> preferencesPages() const;
	/// Returns list of menu actions for elements in palette.
	QList<QAction *> menuActionsList() const;

	/// Returns a multimap of project conveters to editors whoose diagrams they convert.
	/// @warning The result is obtained each time from scratch when you call this method so better to memorize it.
	QMultiMap<QString, ProjectConverter> projectConverters() const;

	/// Returns GUI customizer object. In each QReal's instance there should be only one plugin with
	/// valid customizationInterface(): robots plugin for QReal:Robots, ubiq plugin for QReal:Ubiq etc.
	/// If there're more than one plugin of that kind, it is not specified which one will be used.
	Customizer *customizer() const;

	/// Notifies plugins about change of currently open tab. If last tab is closed or current
	/// tab is not diagram at all (text editor, for example), rootElementId is Id()
	/// @param rootElementId Id of the tab which became active after change, if applicable. If not, Id().
	void activeTabChanged(const Id & rootElementId);

private:
	QList<HotKeyActionInfo> hotKeyActions() const;
	void setHotKeyActions() const;

	/// Asks plugins for custom default settings and merges them with engine`s ones.
	void loadDefaultSettings();

	QDir mPluginsDir;

	QList<InterpretedPluginInterface *> mPlugins;  // doesn't have ownership

	/// An object and that is used to customize GUI with values from plugins
	Customizer mCustomizer;

	/// Common part of all plugin managers
	PluginManager mPluginManager;
};

}

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

#include <plugins/toolPluginInterface/pluginConfigurator.h>
#include "plugins/pluginManager/interpreterEditorManager.h"

#include "pluginsManagerDeclSpec.h"

namespace qReal {

/// This class loads interpreted plugins given model and metamodel.
class QRGUI_PLUGINS_MANAGER_EXPORT InterpretedPluginsLoader
{
public:
	/// Constructor. Also inits interpreted plugins given editor manager interface and plugin configurator.
	/// @param editorManager - list of metamodels
	/// @param configurator - information about models
	InterpretedPluginsLoader(EditorManagerInterface *editorManager
			, const PluginConfigurator &configurator);

	/// Returns list of actions for all plugins.
	QList<ActionInfo> listOfActions() const;

	/// Returns list of additional actions in palette.
	QList<QAction *> menuActionsList() const;

private:
	InterpretedPluginManager mPluginManager;
	QMap<QString, qrRepo::RepoApi *> mEditorRepoApi;  // doesn't have ownership
	InterpreterEditorManager *mInterpreterEditorManager;  // doesn't have ownership
};
}

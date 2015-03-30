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
	InterpretedPluginsLoader();

	/// Inits interpreted plugins given editor manager interface and plugin configurator.
	/// @param editorManager - list of metamodels
	/// @param configurator - information about models
	void init(EditorManagerInterface *editorManager
			, const PluginConfigurator &configurator
			);

	/// Returns list of actions for all plugins.
	QList<ActionInfo> listOfActions() const;

	QList<QAction *> menuActionsList() const;

private:
	InterpretedPluginManager mPluginManager;
	QMap<QString, qrRepo::RepoApi *> mEditorRepoApi;  // doesn't have ownership
	InterpreterEditorManager *mInterpreterEditorManager;
};
}

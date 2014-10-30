#pragma once

#include "plugins/toolPluginInterface/pluginConfigurator.h"
#include "interpreterEditorManager.h"

namespace qReal {

/// This class loads interpreted plugins given model and metamodel.
class InterpretedPluginsLoader
{
public:
	InterpretedPluginsLoader();

	/// Inits interpreted plugins given editor manager interface and plugin configurator.
	/// @param editorManager - list of metamodels
	/// @param configurator - information about models
	void init(EditorManagerInterface *editorManager
			, PluginConfigurator const &configurator
			);

	/// Returns list of actions for all plugins.
	QList<ActionInfo> listOfActions() const;

private:
	InterpretedPluginManager mPluginManager;
	QMap<QString, qrRepo::RepoApi *> mEditorRepoApi;  // doesn't have ownership
	InterpreterEditorManager *mInterpreterEditorManager;
};
}

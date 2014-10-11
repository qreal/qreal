#include "interpretedPluginsLoader.h"

using namespace qReal;

InterpretedPluginsLoader::InterpretedPluginsLoader()
	: mPluginManager()
{
}

void InterpretedPluginsLoader::init(
		EditorManagerInterface *editorManager
		, PluginConfigurator const &configurator)
{
	mInterpreterEditorManager = static_cast<InterpreterEditorManager *>(editorManager);
	mEditorRepoApi = mInterpreterEditorManager->editorRepoApi();

	for (qrRepo::RepoApi * const repo : mEditorRepoApi.values()) {
		mPluginManager.init(configurator, *repo);
	}
}

QList<ActionInfo> InterpretedPluginsLoader::listOfActions() const
{
	QList<ActionInfo> const actions = mPluginManager.actions();
	return actions;
}

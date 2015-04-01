#include "interpretedPluginsLoader.h"

using namespace qReal;

InterpretedPluginsLoader::InterpretedPluginsLoader()
	: mPluginManager()
{
}

void InterpretedPluginsLoader::init(
		EditorManagerInterface *editorManager
		, const PluginConfigurator &configurator)
{
	mInterpreterEditorManager = static_cast<InterpreterEditorManager *>(editorManager);
	mEditorRepoApi = mInterpreterEditorManager->listOfMetamodels();

	for (qrRepo::RepoApi * const repo : mEditorRepoApi.values()) {
		mPluginManager.init(configurator, *repo, editorManager);
	}
}

QList<ActionInfo> InterpretedPluginsLoader::listOfActions() const
{
	return mPluginManager.actions();
}

QList<QAction *> InterpretedPluginsLoader::menuActionsList() const
{
	return mPluginManager.menuActionsList();
}

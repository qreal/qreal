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
	mInterpreterEditorManager = dynamic_cast<InterpreterEditorManager *>(editorManager);
	mEditorRepoApi = mInterpreterEditorManager->editorRepoApi();

	for (qrRepo::RepoApi * const repo : mEditorRepoApi.values()) {
		qrRepo::LogicalRepoApi *logicalRepoApi = dynamic_cast<qrRepo::LogicalRepoApi *>(repo);
		mPluginManager.init(configurator, *logicalRepoApi);
	}
}

QList<ActionInfo> InterpretedPluginsLoader::listOfActions() const
{
	QList<ActionInfo> const actions = mPluginManager.actions();
	return actions;
}

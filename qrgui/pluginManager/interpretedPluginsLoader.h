#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>

#include <qrrepo/graphicalRepoApi.h>
#include <qrrepo/logicalRepoApi.h>
#include <qrrepo/repoApi.h>

#include "toolPluginInterface/pluginConfigurator.h"
#include "interpreterEditorManager.h"

namespace qReal {

/// This class loads interpreted plugins
/// given model and metamodel
class InterpretedPluginsLoader
{
public:
	InterpretedPluginsLoader();
	void init(EditorManagerInterface *editorManager
			, PluginConfigurator const &configurator
			);

	QList<ActionInfo> listOfActions() const;

private:
	InterpretedPluginManager mPluginManager;
	QMap<QString, qrRepo::RepoApi*> mEditorRepoApi;
	InterpreterEditorManager* mInterpreterEditorManager;
};
}

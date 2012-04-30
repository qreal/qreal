#pragma once

#include <QtCore/QPluginLoader>
#include "editorManager.h"
#include "../../qrrepo/logicalRepoApi.h"
#include "../constraintsPluginInterface/constraintsPluginInterface.h"

namespace qReal {

class ConstraintsManager
{
public:
	ConstraintsManager();
	CheckStatus check(Id const &element, qrRepo::LogicalRepoApi const &logicalApi, EditorManager const &editorManager);

private:
	QStringList mPluginsLoaded;
	QMap<QString, QString> mPluginFileName;
	QMap<QString, QPluginLoader *> mLoaders;

	QList<ConstraintsPluginInterface *> mPlugins;
};

}

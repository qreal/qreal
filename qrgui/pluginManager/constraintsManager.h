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
	QList<CheckStatus> check(Id const &element, qrRepo::LogicalRepoApi const &logicalApi, EditorManager const &editorManager);
	bool loadPlugin(QString const &pluginName);
	bool unloadPlugin(QString const &pluginId);
	IdList plugins() const;

private:
	QStringList mPluginsLoaded;
	QMap<QString, QString> mPluginFileName;
	QMap<QString, QPluginLoader *> mLoaders;
	QDir mPluginsDir;

	QList<ConstraintsPluginInterface *> mPlugins;
};

}

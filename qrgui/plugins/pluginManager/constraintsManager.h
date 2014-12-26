#pragma once

#include <QtCore/QPluginLoader>
#include "editorManager.h"
#include "qrrepo/logicalRepoApi.h"
#include "plugins/constraintsPluginInterface/constraintsPluginInterface.h"

namespace qReal {

class QRGUI_PLUGINS_MANAGER_EXPORT ConstraintsManager
{
public:
	ConstraintsManager();
	QList<CheckStatus> check(Id const &element, qrRepo::LogicalRepoApi const &logicalApi, EditorManagerInterface const &editorManager);
	bool loadPlugin(QString const &pluginName);
	bool unloadPlugin(QString const &pluginId);
	IdList pluginsIds() const;
	QList<QString> pluginsNames() const;

private:
	QStringList mPluginsLoaded;
	QMap<QString, QString> mPluginFileName;
	QMap<QString, QPluginLoader *> mLoaders;
	QDir mPluginsDir;

	QMap<QString, ConstraintsPluginInterface *> mPluginIface;
};

}

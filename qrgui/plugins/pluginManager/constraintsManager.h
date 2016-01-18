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

#include <QtCore/QPluginLoader>
#include "editorManager.h"

#include <qrrepo/logicalRepoApi.h>
#include <plugins/constraintsPluginInterface/constraintsPluginInterface.h>

namespace qReal {

class QRGUI_PLUGINS_MANAGER_EXPORT ConstraintsManager
{
public:
	ConstraintsManager();

	QList<CheckStatus> check(
			const Id &element
			, const qrRepo::LogicalRepoApi &logicalApi
			, const EditorManagerInterface &editorManager);

	bool loadPlugin(const QString &pluginName);
	bool unloadPlugin(const QString &pluginId);

	IdList pluginsIds() const;
	QList<QString> pluginsNames() const;

private:
	void insertNewPluginIntoList(
			ConstraintsPluginInterface *pluginInterface
			, const QString &pluginName
			, const QString &pluginId);

	/// List of plugin ids.
	QStringList mListOfPluginIds;

	/// List of pairs (plugin id and plugin name).
	QMap<QString, QString> mPluginIdAndFileName;

	/// List of pairs (plugin id and loaded plugin interfaces).
	QMap<QString, ConstraintsPluginInterface *> mPluginIdAndLoadedPlugins;

	/// Common part of all plugin managers.
	PluginManager mPluginManager;
};

}

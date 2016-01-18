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

#include "constraintsManager.h"

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>
#include <QtCore/QPair>

#include <QtWidgets/QMessageBox>

#include <qrkernel/platformInfo.h>

using namespace qReal;

ConstraintsManager::ConstraintsManager()
	: mPluginManager(PlatformInfo::invariantSettingsPath("pathToToolPlugins"))
{
	const QList<ConstraintsPluginInterface *> loadedPlugins =
			mPluginManager.loadAllPlugins<ConstraintsPluginInterface>();

	for (ConstraintsPluginInterface *constraintPlugin : loadedPlugins) {
		if (constraintPlugin) {
			const QString pluginName = mPluginManager.fileName(constraintPlugin);
			const QString pluginId = constraintPlugin->id();
			insertNewPluginIntoList(constraintPlugin, pluginName, pluginId);
		}
	}
}

bool ConstraintsManager::loadPlugin(const QString &pluginName)
{
	const QPair<ConstraintsPluginInterface *, QString> pluginAndErrorMessage =
			mPluginManager.pluginLoadedByName<ConstraintsPluginInterface>(pluginName);

	ConstraintsPluginInterface *constraintPlugin =  pluginAndErrorMessage.first;
	const QString error = pluginAndErrorMessage.second;

	if (constraintPlugin) {
		const QString pluginId = constraintPlugin->id();
		insertNewPluginIntoList(constraintPlugin, pluginName, pluginId);
	} else {
		QMessageBox::warning(0, "QReal Plugin", error);
	}

	return (error.isEmpty());
}

bool ConstraintsManager::unloadPlugin(const QString &pluginId)
{
	QString errorMessage = "";
	const QString pluginName = mPluginIdAndFileName[pluginId];

	if (!pluginName.isEmpty()) {
		errorMessage = mPluginManager.unloadPlugin(pluginName);

		mPluginIdAndLoadedPlugins.remove(pluginId);
		mListOfPluginIds.removeAll(pluginId);
		mPluginIdAndFileName.remove(pluginId);
	}

	return (errorMessage.isEmpty());
}

IdList ConstraintsManager::pluginsIds() const
{
	IdList plugins;

	for (const QString &pluginId : mListOfPluginIds) {
		plugins.append(Id(pluginId));
	}

	return plugins;
}

QList<QString> ConstraintsManager::pluginsNames() const
{
	return mPluginManager.namesOfPlugins();
}

QList<CheckStatus> ConstraintsManager::check(
		const Id &element
		, const qrRepo::LogicalRepoApi &logicalApi
		, const EditorManagerInterface &editorManager)
{
	QList<qReal::CheckStatus> checkings;

	for (ConstraintsPluginInterface *constraintsInterface : mPluginIdAndLoadedPlugins.values()) {
		if (constraintsInterface->isCorrectMetamodelName(element)) {
			checkings.append(constraintsInterface->check(element, logicalApi, editorManager));
		}
	}

	return checkings;
}

void ConstraintsManager::insertNewPluginIntoList(
		ConstraintsPluginInterface *pluginInterface
		, const QString &pluginName
		, const QString &pluginId)
{
	mListOfPluginIds.append(pluginId);
	mPluginIdAndFileName.insert(pluginId, pluginName);
	mPluginIdAndLoadedPlugins.insert(pluginId, pluginInterface);
}

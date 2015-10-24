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
#include <QtWidgets/QMessageBox>

using namespace qReal;

ConstraintsManager::ConstraintsManager()
{
	mPluginsDir = QDir(qApp->applicationDirPath());

	while (!mPluginsDir.isRoot() && !mPluginsDir.entryList(QDir::Dirs).contains("plugins")) {
		mPluginsDir.cdUp();
	}

	mPluginsDir.cd("plugins");
	mPluginsDir.cd("tools");

	foreach (QString fileName, mPluginsDir.entryList(QDir::Files)) {
		// TODO: Free memory
		QPluginLoader *loader  = new QPluginLoader(mPluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader->instance();

		if (plugin) {
			ConstraintsPluginInterface *constraintsPlugin = qobject_cast<ConstraintsPluginInterface *>(plugin);
			if (constraintsPlugin) {
				mPluginsLoaded += constraintsPlugin->id();
				mPluginFileName.insert(constraintsPlugin->id(), fileName);
				mLoaders.insert(fileName, loader);
				mPluginIface[constraintsPlugin->id()] = constraintsPlugin;
			}
			else {
				loader->unload();
				delete loader;
			}
		} else {
			qDebug() << "Plugin loading failed: " << loader->errorString();
			loader->unload();
			delete loader;
		}
	}
}

bool ConstraintsManager::loadPlugin(const QString &pluginName)
{
	QPluginLoader *loader = new QPluginLoader(mPluginsDir.absoluteFilePath(pluginName));
	loader->load();
	QObject *plugin = loader->instance();

	if (plugin) {
		ConstraintsPluginInterface *constraintsPlugin = qobject_cast<ConstraintsPluginInterface *>(plugin);
		if (constraintsPlugin) {
			mPluginsLoaded += constraintsPlugin->id();
			mPluginFileName.insert(constraintsPlugin->id(), pluginName);
			mLoaders.insert(pluginName, loader);
			mPluginIface[constraintsPlugin->id()] = constraintsPlugin;
			return true;
		}
	}

	QMessageBox::warning(0, "QReal Plugin", loader->errorString());
	loader->unload();
	delete loader;
	return false;
}

bool ConstraintsManager::unloadPlugin(const QString &pluginId)
{
	QString pluginName = mPluginFileName[pluginId];
	QPluginLoader *loader = mLoaders[pluginName];
	if (loader != nullptr) {
		if (!(loader->unload())) {
			delete loader;
			return false;
		}
		mPluginsLoaded.removeAll(pluginId);
		mPluginFileName.remove(pluginId);
		delete loader;
		return true;
	}
	return false;
}

IdList ConstraintsManager::pluginsIds() const
{
	IdList plugins;
	foreach (QString pluginId, mPluginsLoaded) {
		plugins.append(Id(pluginId));
	}
	return plugins;
}

QList<QString> ConstraintsManager::pluginsNames() const
{
	return mPluginFileName.values();
}

QList<CheckStatus> ConstraintsManager::check(
		const Id &element
		, const qrRepo::LogicalRepoApi &logicalApi
		, const EditorManagerInterface &editorManager)
{
	QList<qReal::CheckStatus> checkings;
	foreach (ConstraintsPluginInterface *constraintsInterface, mPluginIface.values()) {
		if (constraintsInterface->isCorrectMetamodelName(element)) {
			checkings.append(constraintsInterface->check(element, logicalApi, editorManager));
		}
	}
	return checkings;
}

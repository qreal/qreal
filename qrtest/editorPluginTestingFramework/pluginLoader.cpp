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

#include "pluginLoader.h"
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>

#include "qrrepo/repoApi.h"
#include "qrutils/nameNormalizer.h"

using namespace qReal;
using namespace editorPluginTestingFramework;
using namespace utils;
using namespace qrRepo;

Metamodel* PluginLoader::loadedPlugin(
		const QString &fileName
		, const QString &pathToFile
		, const QString &pluginExtension
		, const QString &prefix)
{
	qDebug() << "STARTING PLUGIN LOADING";
	QDir mPluginDir = QDir(pathToFile);

	QString normalizedFileName = fileName;
	if (!fileName.contains(".qrs")) {
		normalizedFileName += ".qrs";
	}

	const RepoApi *mRepoApi = new RepoApi(normalizedFileName);
	const IdList metamodels = mRepoApi->children(Id::rootId());

	for (Id const &key : metamodels) {
		if (mRepoApi->isLogicalElement(key)) {
			const QString &normalizedMetamodelName = NameNormalizer::normalize(mRepoApi->stringProperty(key, "name"), false);
			const QString &pluginName = prefix + normalizedMetamodelName + "-d" + "." + pluginExtension;
			mPluginNames.append(pluginName);

			QPluginLoader * const loader = new QPluginLoader(mPluginDir.absoluteFilePath(pluginName));
			loader->load();
			QObject *plugin = loader->instance();

			if (plugin) {
				qDebug() << "plugin is loaded";
				Metamodel * const iEditor = qobject_cast<Metamodel *>(plugin);
				return iEditor;
			}

			qDebug() << "plugin is NOT loaded";
		}
	}

	return nullptr;
}

QStringList PluginLoader::pluginNames()
{
	return mPluginNames;
}

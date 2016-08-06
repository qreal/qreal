/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

#include <metaMetaModel/metamodel.h>

#include "defs.h"

using namespace editorPluginTestingFramework;

PluginLoader::~PluginLoader()
{
}

qReal::Metamodel* PluginLoader::loadPlugin(const QFileInfo &file)
{
	qDebug() << "Loading plugin...";

	const QSharedPointer<QPluginLoader> loader(new QPluginLoader(file.canonicalFilePath()));
	mLoadedPluginLoaders.append(loader);
	loader->load();
	QObject *plugin = loader->instance();

	if (plugin) {
		qDebug() << "Plugin loaded successfully";
		qReal::Metamodel * const pluginInterface = qobject_cast<qReal::Metamodel *>(plugin);
		qDebug() << stringSeparator;
		return pluginInterface;
	}

	qDebug() << "Plugin loading failed";
	mLoadedPluginLoaders.removeAll(loader);

	return nullptr;
}

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

#include "pluginManagerImplementation.h"

#include <QtCore/QCoreApplication>

#include <qrkernel/logging.h>

using namespace qReal::details;

PluginManagerImplementation::PluginManagerImplementation(const QString &pluginsDirPath)
	: mPluginsDir(pluginsDirPath)
{
}

PluginManagerImplementation::~PluginManagerImplementation()
{
	for (auto &pair : mLoaders) {
		pair.second->unload();
		delete pair.second;
	}
}

QList<QObject *> PluginManagerImplementation::loadAllPlugins()
{
	if (!mPluginsDir.exists()) {
		QLOG_INFO() << "Plugins directory" << mPluginsDir.path()
				<< "does not exist, maybe we are in 'metamodeling on fly' mode";
		return {};
	}

	QList<QObject *> listOfPlugins;

	for (const QString &fileName : mPluginsDir.entryList(QDir::Files)) {
		QPair<QObject *, QString> const pluginAndError =  loadPluginByName(fileName);
		QObject * const pluginByName = pluginAndError.first;
		if (pluginByName) {
			listOfPlugins.append(pluginByName);
			mFileNameAndPlugin.insert(fileName, pluginByName);
		} else {
			QLOG_ERROR() << "Plugin loading failed:" << pluginAndError.second;
		}
	}

	return listOfPlugins;
}

QPair<QObject *, QString> PluginManagerImplementation::loadPluginByName(const QString &pluginName)
{
	QPluginLoader * const loader = new QPluginLoader(mPluginsDir.absoluteFilePath(pluginName), qApp);
	loader->load();
	QObject * const plugin = loader->instance();

	if (plugin) {
		mLoaders.append(qMakePair(pluginName, loader));
		mFileNameAndPlugin.insert(loader->metaData()["IID"].toString(), plugin);
		return qMakePair(plugin, QString());
	}

	const QString loaderError = loader->errorString();

	// Unloading of plugins is currently (Qt 5.3) broken due to a bug in metatype system: calling Q_DECLARE_METATYPE
	// from plugin registers some data from plugin address space in Qt metatype system, which is not being updated
	// when plugin is unloaded and loaded again. Any subsequent calls to QVariant or other template classes/methods
	// which use metainformation will result in a crash. It is likely (but not verified) that qRegisterMetaType leads
	// to the same issue. Since we can not guarantee that plugin does not use Q_DECLARE_METATYPE or qRegisterMetaType
	// we shall not unload plugin at all, to be safe rather than sorry.
	//
	// But it seems also that metainformation gets deleted BEFORE plugins are unloaded on application exit, so we can
	// not call any metainformation-using code in destructors that get called on unloading. Since Qt classes themselves
	// are using such calls (QGraphicsViewScene, for example), random crashes on exit may be a symptom of this problem.
	//
	// EditorManager is an exception, because it really needs to unload editor plugins, to be able to load new versions
	// compiled by metaeditor. Editor plugins are generated, so we can guarantee to some extent that there will be no
	// metatype registrations.
	//
	// See:
	// http://stackoverflow.com/questions/19234703/using-q-declare-metatype-with-a-dll-that-may-be-loaded-multiple-times
	// (and http://qt-project.org/forums/viewthread/35587)
	// https://bugreports.qt-project.org/browse/QTBUG-32442

	delete loader;
	return qMakePair(nullptr, loaderError);
}

QString PluginManagerImplementation::unloadPlugin(const QString &pluginName)
{
	int count = 0;
	bool stateUnload = true;

	for (const QPair<QString, QPluginLoader *> &currentPair : mLoaders) {
		if (currentPair.first == pluginName) {
			stateUnload = currentPair.second->unload();
		}

		++count;
	}

	if (stateUnload) {
		return QString();
	}

	return QString("Plugin was not found");
}

QList<QString> PluginManagerImplementation::namesOfPlugins() const
{
	QList<QString> listOfNames;
	for (const QPair<QString, QPluginLoader *> &currentPair : mLoaders) {
		listOfNames.append(currentPair.first);
	}

	return listOfNames;
}

QString PluginManagerImplementation::fileName(QObject *plugin) const
{
	return mFileNameAndPlugin.key(plugin);
}

QObject *PluginManagerImplementation::pluginByName(const QString &pluginName) const
{
	return mFileNameAndPlugin[pluginName];
}

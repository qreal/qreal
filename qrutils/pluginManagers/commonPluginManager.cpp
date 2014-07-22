#include "commonPluginManager.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>

using namespace qReal;

CommonPluginManager::CommonPluginManager(QString const &applicationDirPath) :
	mPluginsDir(QDir(applicationDirPath))
	, mApplicationDirectoryPath(applicationDirPath)
{
}

CommonPluginManager::~CommonPluginManager()
{
	qDeleteAll(mLoaders);
}

QList<QObject *> CommonPluginManager::allLoadedPlugins()
{
	while (!mPluginsDir.isRoot() && !mPluginsDir.entryList(QDir::Dirs).contains("plugins")) {
		mPluginsDir.cdUp();
	}

	mPluginsDir.cd("plugins");

	QList<QObject *> listOfPlugins;

	foreach (QString const &fileName, mPluginsDir.entryList(QDir::Files)) {
		QPluginLoader *loader  = new QPluginLoader(mPluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader->instance();

		if (plugin) {
			mLoaders.insert(fileName, loader);
			mNameAndObject.insert(fileName, plugin);

			listOfPlugins.append(plugin);
		} else {
			// they say it doesn't work
			// i believe...
			loader->unload();
			delete loader;
		}
	}

	return listOfPlugins;
}

void CommonPluginManager::deleteAllLoaders()
{
	qDeleteAll(mLoaders);
}

QObject* CommonPluginManager::pluginLoadedByName(QString const &pluginName)
{
	QPluginLoader *loader = new QPluginLoader(mPluginsDir.absoluteFilePath(pluginName));
	loader->load();
	QObject *plugin = loader->instance();

	if (plugin) {
		return plugin;
	}

	QMessageBox::warning(NULL, "error", "Plugin loading failed: " + loader->errorString());
	loader->unload();
	delete loader;

	return NULL;
}

QPair<bool, bool> CommonPluginManager::unloadPlugin(QString const &pluginName)
{
	QPluginLoader *loader = mLoaders[pluginName];

	if (loader != NULL) {
		mLoaders.remove(pluginName);
		if (!loader->unload()) {
			QMessageBox::warning(NULL, "error", "Plugin unloading failed: " + loader->errorString());
			delete loader;
			return qMakePair(false, true);
		}
		delete loader;
		return qMakePair(true, true);
	}

	return qMakePair(false, false);
}

QString CommonPluginManager::fileName(QObject *plugin) const
{
	return mNameAndObject.key(plugin);
}


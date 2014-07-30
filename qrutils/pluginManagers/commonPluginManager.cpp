#include "commonPluginManager.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>

using namespace qReal;

CommonPluginManager::CommonPluginManager(QString const &applicationDirPath
		, QString const &additionalPart) :
	mPluginsDir(QDir(applicationDirPath))
	, mApplicationDirectoryPath(applicationDirPath)
	, mAdditionalPart(additionalPart)
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

	QList<QString> splittedDir = mAdditionalPart.split('/');
	foreach (QString const &partOfDirectory, splittedDir) {
		mPluginsDir.cd(partOfDirectory);
	}

	QList<QObject *> listOfPlugins;

	foreach (QString const &fileName, mPluginsDir.entryList(QDir::Files)) {
		QObject * pluginByName = pluginLoadedByName(fileName).first;
		if (pluginByName) {
			listOfPlugins.append(pluginByName);
		}
	}

	return listOfPlugins;
}

void CommonPluginManager::deleteAllLoaders()
{
	qDeleteAll(mLoaders);
}

QPair<QObject *, QString> CommonPluginManager::pluginLoadedByName(QString const &pluginName)
{
	QPluginLoader *loader = new QPluginLoader(mPluginsDir.absoluteFilePath(pluginName));
	loader->load();
	QObject *plugin = loader->instance();

	if (plugin) {
		return qMakePair(plugin, QString());
	}

	QString const loaderError = loader->errorString();
	delete loader;
	return qMakePair(nullptr, loaderError);
}

QPair<QString, QPair<bool, bool> > CommonPluginManager::unloadPlugin(QString const &pluginName)
{
	QPluginLoader *loader = mLoaders[pluginName];

	if (loader) {
		mLoaders.remove(pluginName);
		if (!loader->unload()) {
			QString const error = loader->errorString();
			delete loader;
			return qMakePair(error, qMakePair(false, true));
		}

		delete loader;
		return qMakePair(QString(), qMakePair(true, true));
	}

	return qMakePair(QString("Plugin was not found"), qMakePair(false, false));
}

QString CommonPluginManager::fileName(QObject *plugin) const
{
	return mNameAndObject.key(plugin);
}

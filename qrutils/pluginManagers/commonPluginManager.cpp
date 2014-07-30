#include "commonPluginManager.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>

using namespace qReal;

CommonPluginManager::CommonPluginManager(QString const &applicationDirPath
		, QString const &additionalPart)
	: mPluginsDir(QDir(applicationDirPath))
	, mApplicationDirectoryPath(applicationDirPath)
	, mAdditionalPart(additionalPart)
{
}

CommonPluginManager::~CommonPluginManager()
{
	qDeleteAll(mLoaders);
}

QList<QObject *> CommonPluginManager::loadAllPlugins()
{
	while (!mPluginsDir.isRoot() && !mPluginsDir.entryList(QDir::Dirs).contains("plugins")) {
		mPluginsDir.cdUp();
	}

	QList<QString> splittedDir = mAdditionalPart.split('/');
	for (QString const &partOfDirectory : splittedDir) {
		mPluginsDir.cd(partOfDirectory);
	}

	QList<QObject *> listOfPlugins;

	for (QString const &fileName : mPluginsDir.entryList(QDir::Files)) {
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

QString CommonPluginManager::unloadPlugin(QString const &pluginName)
{
	QPluginLoader *loader = mLoaders[pluginName];

	if (loader) {
		mLoaders.remove(pluginName);

		if (!loader->unload()) {
			QString const error = loader->errorString();
			delete loader;
			return error;
		}

		delete loader;
		return QString();
	}

	return QString("Plugin was not found");
}

QString CommonPluginManager::fileName(QObject *plugin) const
{
	return mNameAndObject.key(plugin);
}

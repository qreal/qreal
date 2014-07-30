#include "kitPluginManager.h"

#include <QtCore/QFileInfo>
#include <QtCore/QCoreApplication>

#include <qrkernel/exception/exception.h>

#include <qrutils/pluginManagers/interfaceWrapper.h>

using namespace interpreterCore;
using namespace qReal;

KitPluginManager::KitPluginManager(QString const &pluginDirectory)
	: mPluginsDir(QCoreApplication::applicationDirPath() + "/" + pluginDirectory)
{
	for (QString const &fileName : mPluginsDir.entryList(QDir::Files)) {
		QFileInfo const fileInfo(fileName);

		if (fileInfo.suffix() != "dll" && fileInfo.suffix() != "so") {
			continue;
		}

		QPluginLoader * const loader  = new QPluginLoader(mPluginsDir.absoluteFilePath(fileName));
		QObject * const plugin = loader->instance();

		if (plugin) {
			if (tryToLoadInterpreterPlugin(plugin) || tryToLoadGeneratorPlugin(plugin)) {
				mLoaders.insert(fileName, loader);
			} else {
				// We can't unload plugins due to Qt bug, see editorManager.cpp for explanation.
				// loader->unload();
				delete loader;
			}
		} else {
			qDebug() << "Plugin loading failed: " << loader->errorString();
//			loader->unload();
			delete loader;
		}
	}
	mCommonPluginManager = new CommonPluginManager(QCoreApplication::applicationDirPath(), pluginDirectory);
	tryToLoadGeneratorPlugin();
	tryToLoadInterpreterPlugin();
}

KitPluginManager::~KitPluginManager()
{
	// We can't unload plugins due to Qt bug, see editorManager.cpp for explanation.
	// for (QPluginLoader * const loader : mLoaders.values()) {
	//	loader->unload();
	// }

	mCommonPluginManager->deleteAllLoaders();
	qDeleteAll(mLoaders);
}

QList<QString> KitPluginManager::kitIds() const
{
	return mPluginInterfaces.keys();
}

QList<interpreterBase::KitPluginInterface *> KitPluginManager::kitsById(QString const &kitId) const
{
	if (!mPluginInterfaces.contains(kitId)) {
		throw qReal::Exception("Requesting non-existing kit plugin");
	}

	return mPluginInterfaces.values(kitId);
}

QList<generatorBase::GeneratorKitPluginInterface *> KitPluginManager::generatorsById(QString const &kitId) const
{
	// There can be no generators for the given kit id, so we do not filter out non-existing case here.
	return mGenerators.values(kitId);
}

QList<interpreterBase::robotModel::RobotModelInterface *> KitPluginManager::allRobotModels() const
{
	QList<interpreterBase::robotModel::RobotModelInterface *> result;
	for (interpreterBase::KitPluginInterface * const kit : mPluginInterfaces) {
		result += kit->robotModels();
	}

	return result;
}

void KitPluginManager::tryToLoadInterpreterPlugin()
{
	QList<interpreterBase::KitPluginInterface *> loadedInterpreterPlugins =
			InterfaceWrapper<interpreterBase::KitPluginInterface>::listOfInterfaces(mCommonPluginManager->allLoadedPlugins());

	foreach (interpreterBase::KitPluginInterface * const kitPlugin, loadedInterpreterPlugins) {
		mPluginInterfaces.insertMulti(kitPlugin->kitId(), kitPlugin);
	}
}

void KitPluginManager::tryToLoadGeneratorPlugin()
{
	QList<generatorBase::GeneratorKitPluginInterface *> loadedGeneratorPlugins =
			InterfaceWrapper<generatorBase::GeneratorKitPluginInterface>::listOfInterfaces(mCommonPluginManager->allLoadedPlugins());

	foreach (generatorBase::GeneratorKitPluginInterface * const generatorPlugin, loadedGeneratorPlugins) {
		mGenerators.insertMulti(generatorPlugin->kitId(), generatorPlugin);
	}
}

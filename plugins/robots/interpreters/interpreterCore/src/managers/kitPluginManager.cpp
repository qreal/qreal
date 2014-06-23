#include "kitPluginManager.h"

#include <QtCore/QFileInfo>
#include <QtCore/QCoreApplication>

#include <qrkernel/exception/exception.h>

using namespace interpreterCore;

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
				// loader->unload();
				delete loader;
			}
		} else {
			qDebug() << "Plugin loading failed: " << loader->errorString();
			loader->unload();
			delete loader;
		}
	}
}

KitPluginManager::~KitPluginManager()
{
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

bool KitPluginManager::tryToLoadInterpreterPlugin(QObject * const plugin)
{
	interpreterBase::KitPluginInterface * const kitPlugin
			= qobject_cast<interpreterBase::KitPluginInterface *>(plugin);

	if (kitPlugin) {
		mPluginInterfaces.insertMulti(kitPlugin->kitId(), kitPlugin);
	}

	return kitPlugin != nullptr;
}

bool KitPluginManager::tryToLoadGeneratorPlugin(QObject * const plugin)
{
	generatorBase::GeneratorKitPluginInterface * const generatorPlugin
			= qobject_cast<generatorBase::GeneratorKitPluginInterface *>(plugin);
	if (generatorPlugin) {
		mGenerators.insertMulti(generatorPlugin->kitId(), generatorPlugin);
	}

	return generatorPlugin != nullptr;
}

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
	mCommonPluginManager = new CommonPluginManager(QCoreApplication::applicationDirPath(), pluginDirectory);
	tryToLoadGeneratorPlugins();
	tryToLoadInterpreterPlugins();
}

KitPluginManager::~KitPluginManager()
{
	mCommonPluginManager->deleteAllLoaders();
	delete(mCommonPluginManager);
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

void KitPluginManager::tryToLoadInterpreterPlugins()
{
	QList<interpreterBase::KitPluginInterface *> loadedInterpreterPlugins =
			InterfaceWrapper<interpreterBase::KitPluginInterface>::listOfInterfaces(mCommonPluginManager->allLoadedPlugins());

	for (interpreterBase::KitPluginInterface * const kitPlugin: loadedInterpreterPlugins) {
		mPluginInterfaces.insertMulti(kitPlugin->kitId(), kitPlugin);
	}
}

void KitPluginManager::tryToLoadGeneratorPlugins()
{
	QList<generatorBase::GeneratorKitPluginInterface *> loadedGeneratorPlugins =
			InterfaceWrapper<generatorBase::GeneratorKitPluginInterface>::listOfInterfaces(mCommonPluginManager->allLoadedPlugins());

	for (generatorBase::GeneratorKitPluginInterface * const generatorPlugin: loadedGeneratorPlugins) {
		mGenerators.insertMulti(generatorPlugin->kitId(), generatorPlugin);
	}
}

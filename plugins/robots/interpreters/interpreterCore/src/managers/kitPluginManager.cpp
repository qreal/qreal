#include "kitPluginManager.h"

#include <QtCore/QFileInfo>
#include <QtCore/QCoreApplication>

#include <qrkernel/exception/exception.h>

using namespace interpreterCore;
using namespace qReal;

KitPluginManager::KitPluginManager(QString const &pluginDirectory)
	: mPluginsDir(QCoreApplication::applicationDirPath() + "/" + pluginDirectory)
	, mPluginManager(PluginManager(QCoreApplication::applicationDirPath()))
{
	tryToLoadGeneratorPlugins();
	tryToLoadInterpreterPlugins();
}

KitPluginManager::~KitPluginManager()
{
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
	QList<interpreterBase::KitPluginInterface *> const loadedInterpreterPlugins =
			mPluginManager.loadAllPlugins<interpreterBase::KitPluginInterface>();

	for (interpreterBase::KitPluginInterface * const kitPlugin : loadedInterpreterPlugins) {
		mPluginInterfaces.insertMulti(kitPlugin->kitId(), kitPlugin);
	}
}

void KitPluginManager::tryToLoadGeneratorPlugins()
{
	QList<generatorBase::GeneratorKitPluginInterface *> const loadedGeneratorPlugins =
			mPluginManager.loadAllPlugins<generatorBase::GeneratorKitPluginInterface>();

	for (generatorBase::GeneratorKitPluginInterface * const generatorPlugin : loadedGeneratorPlugins) {
		mGenerators.insertMulti(generatorPlugin->kitId(), generatorPlugin);
	}
}

#include "interpreterCore/managers/kitPluginManager.h"

#include <QtCore/QFileInfo>
#include <QtCore/QCoreApplication>

#include <qrkernel/exception/exception.h>

using namespace interpreterCore;
using namespace qReal;

KitPluginManager::KitPluginManager(const QString &pluginDirectory)
	: mPluginsDir(QCoreApplication::applicationDirPath() + "/" + pluginDirectory)
	, mPluginManager(PluginManager(QCoreApplication::applicationDirPath(), pluginDirectory))
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

QList<kitBase::KitPluginInterface *> KitPluginManager::kitsById(const QString &kitId) const
{
	if (!mPluginInterfaces.contains(kitId)) {
		throw qReal::Exception("Requesting non-existing kit plugin");
	}

	return mPluginInterfaces.values(kitId);
}

QList<generatorBase::GeneratorKitPluginInterface *> KitPluginManager::generatorsById(const QString &kitId) const
{
	// There can be no generators for the given kit id, so we do not filter out non-existing case here.
	return mGenerators.values(kitId);
}

QList<kitBase::robotModel::RobotModelInterface *> KitPluginManager::allRobotModels() const
{
	QList<kitBase::robotModel::RobotModelInterface *> result;
	for (kitBase::KitPluginInterface * const kit : mPluginInterfaces) {
		result += kit->robotModels();
	}

	return result;
}

void KitPluginManager::tryToLoadInterpreterPlugins()
{
	QList<kitBase::KitPluginInterface *> const loadedInterpreterPlugins =
			mPluginManager.loadAllPlugins<kitBase::KitPluginInterface>();

	for (kitBase::KitPluginInterface * const kitPlugin : loadedInterpreterPlugins) {
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

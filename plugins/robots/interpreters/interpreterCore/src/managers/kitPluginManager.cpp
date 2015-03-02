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
	tryToLoadKitPlugins();
}

KitPluginManager::~KitPluginManager()
{
}

QList<QString> KitPluginManager::kitIds() const
{
	return mPluginInterfaces.uniqueKeys();
}

QList<kitBase::KitPluginInterface *> KitPluginManager::kitsById(const QString &kitId) const
{
	if (!mPluginInterfaces.contains(kitId)) {
		throw qReal::Exception("Requesting non-existing kit plugin");
	}

	return mPluginInterfaces.values(kitId);
}

QList<kitBase::robotModel::RobotModelInterface *> KitPluginManager::allRobotModels() const
{
	QList<kitBase::robotModel::RobotModelInterface *> result;
	for (kitBase::KitPluginInterface * const kit : mPluginInterfaces) {
		result += kit->robotModels();
	}

	return result;
}

void KitPluginManager::tryToLoadKitPlugins()
{
	QList<kitBase::KitPluginInterface *> const loadedInterpreterPlugins =
			mPluginManager.loadAllPlugins<kitBase::KitPluginInterface>();

	for (kitBase::KitPluginInterface * const kitPlugin : loadedInterpreterPlugins) {
		mPluginInterfaces.insertMulti(kitPlugin->kitId(), kitPlugin);
	}
}

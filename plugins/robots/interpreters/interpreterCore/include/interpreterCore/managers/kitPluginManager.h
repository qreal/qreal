#pragma once

#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>

#include <kitBase/kitPluginInterface.h>
#include <qrutils/pluginManagers/pluginManager.h>

namespace interpreterCore {

/// Loads kit plugins and provides interface to access them for the rest of interpreterCore.
class KitPluginManager
{
public:
	/// Constructor.
	/// @param pluginDirectory - directory where we need to search for plugins.
	explicit KitPluginManager(const QString &pluginDirectory);

	~KitPluginManager();

	/// Returns a list if ids of loaded kits.
	QList<QString> kitIds() const;

	/// Returns a list of kits that have given id. Note that multiple kits can have one id, in that case their
	/// functionality is merged (needed to support language extensions like support for segway).
	QList<kitBase::KitPluginInterface *> kitsById(const QString &kitId) const;

	/// A convenience method that travels around all loaded kit plugins, collects all robot models and returns them.
	QList<kitBase::robotModel::RobotModelInterface *> allRobotModels() const;

private:
	void tryToLoadKitPlugins();

	/// Maps kit plugin name to corresponding plugin interface.
	/// Doesn't have ownership, objects are owned by mLoaders.
	QMap<QString, kitBase::KitPluginInterface *> mPluginInterfaces;  // Has ownership

	/// Directory from which plugins shall be loaded.
	QDir mPluginsDir;

	/// Common part of plugins loading
	qReal::PluginManager mPluginManager;
};

}

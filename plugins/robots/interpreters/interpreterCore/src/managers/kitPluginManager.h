#pragma once

#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>

#include <interpreterBase/kitPluginInterface.h>

namespace interpreterCore {

/// Loads kit plugins and provides interface to access them for the rest of interpreterCore.
class KitPluginManager
{
public:
	/// Constructor.
	/// @param pluginDirectory - directory where we need to search for plugins.
	explicit KitPluginManager(QString const &pluginDirectory);

	~KitPluginManager();

	QList<QString> kitIds() const;

	interpreterBase::KitPluginInterface &kitById(QString const &kitId);

	/// A convenience method that travels around all loaded kit plugins, collects all robot models and returns them.
	QList<interpreterBase::robotModel::RobotModelInterface *> allRobotModels() const;

	/// @todo Return a notion of selected kit, it is needed to hide actions on a toolbar from all kits except selected.
private:
	/// Maps kit plugin name to corresponding plugin interface.
	/// Doesn't have ownership, objects are owned by mLoaders.
	QMap<QString, interpreterBase::KitPluginInterface *> mPluginInterfaces;

	/// Maps kit plugin name to corresponding loader.
	QMap<QString, QPluginLoader *> mLoaders;  // Has ownership.

	/// Directory from which plugins shall be loaded.
	QDir mPluginsDir;
};

}

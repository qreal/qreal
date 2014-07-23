#pragma once

#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>

#include <interpreterBase/kitPluginInterface.h>
#include <generatorBase/generatorKitPluginInterface.h>

#include <qrutils/pluginManagers/commonPluginManager.h>

namespace interpreterCore {

/// Loads kit plugins and provides interface to access them for the rest of interpreterCore.
class KitPluginManager
{
public:
	/// Constructor.
	/// @param pluginDirectory - directory where we need to search for plugins.
	explicit KitPluginManager(QString const &pluginDirectory);

	~KitPluginManager();

	/// Returns a list if ids of loaded kits.
	QList<QString> kitIds() const;

	/// Returns a list of kits that have given id. Note that multiple kits can have one id, in that case their
	/// functionality is merged (needed to support language extensions like support for segway).
	QList<interpreterBase::KitPluginInterface *> kitsById(QString const &kitId) const;

	/// Returns a list of generators associated with given kit id.
	QList<generatorBase::GeneratorKitPluginInterface *> generatorsById(QString const &kitId) const;

	/// A convenience method that travels around all loaded kit plugins, collects all robot models and returns them.
	QList<interpreterBase::robotModel::RobotModelInterface *> allRobotModels() const;

private:
	void tryToLoadInterpreterPlugin();
	void tryToLoadGeneratorPlugin();

	/// Maps kit plugin name to corresponding plugin interface.
	/// Doesn't have ownership, objects are owned by mLoaders.
	QMap<QString, interpreterBase::KitPluginInterface *> mPluginInterfaces;  // Has ownership

	/// Maps kit plugin name to corresponding loader.
	QMap<QString, generatorBase::GeneratorKitPluginInterface *> mGenerators;  // Has ownership

	/// Maps kit plugin name to corresponding loader.
	QMap<QString, QPluginLoader *> mLoaders;  // Has ownership.

	/// Directory from which plugins shall be loaded.
	QDir mPluginsDir;

	qReal::CommonPluginManager *mCommonPluginManager;
};

}

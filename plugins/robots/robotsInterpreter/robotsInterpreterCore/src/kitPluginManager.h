#pragma once

#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>

#include <robotsInterpreterCore/kitPluginInterface.h>

#include "robotsSettingsPageExtensionsInterface.h"

namespace robotsInterpreterCore {

/// Loads kit plugins and provides interface to access them for the rest of RobotsInterpreterCore.
class KitPluginManager : public RobotsSettingsPageExtensionsInterface
{
public:
	/// Constructor.
	/// @param pluginDirectory - directory where we need to search for plugins.
	explicit KitPluginManager(QString const &pluginDirectory);

	~KitPluginManager();

	// Override.
	virtual QList<QString> kitIds() const;

	// Override.
	// Transfers ownership.
	virtual QWidget *kitSpecificSettingsWidget(QString const &kitId) const;

	// Override.
	virtual qReal::IdList specificBlocks(QString const &kitId) const;

	// Override.
	virtual qReal::IdList unsupportedBlocks(QString const &kitId) const;

private:
	/// Maps kit plugin name to corresponding plugin interface.
	QMap<QString, KitPluginInterface *> mPluginInterfaces;  // Doesn't have ownership, objects are owned by mLoaders.

	/// Maps kit plugin name to corresponding loader.
	QMap<QString, QPluginLoader *> mLoaders;  // Has ownership.

	/// Directory from which plugins shall be loaded.
	QDir mPluginsDir;
};

}

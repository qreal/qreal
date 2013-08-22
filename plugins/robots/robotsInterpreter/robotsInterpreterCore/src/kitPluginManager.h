#pragma once

#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QMap>

#include <robotsInterpreterCore/kitPluginInterface.h>

#include "robotsSettingsPageExtensionsInterface.h"

namespace robotsInterpreterCore {

class KitPluginManager : public RobotsSettingsPageExtensionsInterface
{
public:
	/// Constructor.
	/// @param pluginDirectory - directory where we need to search for plugins.
	explicit KitPluginManager(QString const &pluginDirectory);

	~KitPluginManager();

	// Override.
	virtual QList<QString> kitNames() const;

	// Override.
	// Transfers ownership.
	virtual QWidget *kitSpecificSettingsWidget(QString const &kitName) const;

private:
	QMap<QString, QString> mPluginFileNames;
	QMap<QString, KitPluginInterface *> mPluginInterfaces;  // Has ownership.
	QMap<QString, QPluginLoader *> mLoaders;  // Has ownership.

	QDir mPluginsDir;
};

}


/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	/// @param pluginsDirectory - directory where we need to search for plugins.
	explicit KitPluginManager(const QString &pluginsDirectory);

	~KitPluginManager();

	/// Returns a list if ids of loaded kits.
	QList<QString> kitIds() const;

	/// Returns a list of kits that have given id. Note that multiple kits can have one id, in that case their
	/// functionality is merged (needed to support language extensions like support for segway).
	QList<kitBase::KitPluginInterface *> kitsById(const QString &kitId) const;

	/// A convenience method that travels around all loaded kit plugins, collects all robot models and returns them.
	QList<kitBase::robotModel::RobotModelInterface *> allRobotModels() const;

	/// Returns the priority of the kit with the given id. The priority is the maximum of KitPluginInterface::priority.
	int priority(const QString &kitId) const;

private:
	void tryToLoadKitPlugins();

	/// Maps kit plugin name to corresponding plugin interface.
	/// Doesn't have ownership, objects are owned by mLoaders.
	QMap<QString, kitBase::KitPluginInterface *> mPluginInterfaces;  // Has ownership

	/// Common part of plugins loading
	qReal::PluginManager mPluginManager;
};

}

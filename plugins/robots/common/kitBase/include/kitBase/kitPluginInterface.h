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

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtWidgets/QWidget>

#include <qrkernel/ids.h>
#include <qrgui/plugins/toolPluginInterface/actionInfo.h>
#include <qrgui/plugins/toolPluginInterface/hotKeyActionInfo.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>

#include <kitBase/kitPluginConfigurator.h>
#include <kitBase/additionalPreferences.h>
#include <kitBase/devicesConfigurationProvider.h>
#include <kitBase/blocksBase/blocksFactoryInterface.h>

namespace kitBase {

/// Plugin interface for a kit support plugin.
class KitPluginInterface
{
public:
	virtual ~KitPluginInterface() {}

	/// Passes to kit plugin objects that allow it to communicate with engine.
	virtual void init(const KitPluginConfigurator &configurator)
	{
		Q_UNUSED(configurator)
	}

	/// An identifier of constructor kit. Kit plugins with same kitId are automaticly grouped
	/// together extending each other.
	virtual QString kitId() const = 0;

	/// String that will be displayed to users as the name of the kit. If more than one kit plugins with the same
	/// id implemented in system friendly name can be specified only in one of them.
	virtual QString friendlyKitName() const = 0;

	/// Returns a list of robot models supported by this kit plugin.
	virtual QList<robotModel::RobotModelInterface *> robotModels() = 0;

	/// Returns a blocks factory for a given robot model. Nullptr can be returned and means that no factory is provided
	/// by this plugin. Ownership must be transfered.
	virtual blocksBase::BlocksFactoryInterface *blocksFactoryFor(const robotModel::RobotModelInterface *model) = 0;

	/// If overrided and returns some model as value that model will be used as default selected model for this kit
	/// @todo is it possible that plugin does not provide default model? Maybe first model in robotModels() list will be
	/// enough?
	virtual robotModel::RobotModelInterface *defaultRobotModel()
	{
		return nullptr;
	}

	/// returned no widget is added on settings tab
	/// Widget with specific settings for a plugin. save() method is called when user saves all settings,
	/// restoreSettings() - each time when used selects other robot model for this kit. If nullptr is
	/// Transfers ownership.
	virtual QList<AdditionalPreferences *> settingsWidgets() = 0;

	/// Returns a widget that will be placed at devices configuration widget in the left-hand side dock.
	/// The default implementation returns nullptr.
	/// Transfers ownership.
	virtual QWidget *quickPreferencesFor(const robotModel::RobotModelInterface &model)
	{
		Q_UNUSED(model)
		return nullptr;
	}

	/// List of additional actions supported by plugin, to be added to toolbar and menus. Showing 2d model widget
	/// goes here.
	/// @todo: what about ownership of underlying QAction objects?
	virtual QList<qReal::ActionInfo> customActions() = 0;

	/// Shall be overridden to return QAction instances for their customization in hot key manager.
	virtual QList<qReal::HotKeyActionInfo> hotKeyActions() = 0;

	/// Can be overridden to return a path to a file in INI format with default settings for kit plugin.
	/// If empty string is returned (like in default implementation) default settings are ignored for this kit plugin.
	virtual QString defaultSettingsFile() const
	{
		return QString();
	}

	/// Returns an icon for toolbar action of fast robot model switching.
	/// It is guaranteed that this method will be called with the robot model provided by this kit.
	virtual QIcon iconForFastSelector(const robotModel::RobotModelInterface &robotModel) const = 0;

	/// Sensor configuration provider object for this plugin that needs to be registered in providers network.
	/// Can be a root of plugin-specific providers network. Null, if there is no need for sensors configuration.
	/// Does not transfer ownership.
	virtual kitBase::DevicesConfigurationProvider * devicesConfigurationProvider()
	{
		return nullptr;
	}

	/// Returns kit plugin priority over other plugins. Priority will be used by kits auto switching system
	/// when switching can be performed to multiple kits. If priorities are equal the selected kit is undefined.
	/// Returns 0 if not overrided.
	virtual int priority() const
	{
		return 0;
	}
};

}

Q_DECLARE_INTERFACE(kitBase::KitPluginInterface, "ru.spbsu.math.QReal.KitPluginInterface/1")

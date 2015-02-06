#pragma once

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtWidgets/QWidget>

#include <qrkernel/ids.h>
#include <qrgui/plugins/toolPluginInterface/actionInfo.h>
#include <qrgui/plugins/toolPluginInterface/hotKeyActionInfo.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>

#include <interpreterBase/additionalPreferences.h>
#include <interpreterBase/devicesConfigurationProvider.h>
#include <interpreterBase/eventsForKitPluginInterface.h>
#include <interpreterBase/interpreterControlInterface.h>
#include <interpreterBase/robotModel/robotModelInterface.h>
#include <interpreterBase/blocksBase/blocksFactoryInterface.h>

namespace interpreterBase {

/// Plugin interface for a kit support plugin.
class KitPluginInterface
{
public:
	virtual ~KitPluginInterface() {}

	/// Passes to kit plugin objects that allow it to communicate with engine.
	/// @param eventsForKitPlugin - object with events raised in interpreter that plugin can use to perform custom
	///        actions, for example, starting and stopping of interpretation.
	/// @param systemEvents - object with events from qrgui, like tab switching.
	/// @param interpreterControl - interface with methods that allow plugin to control interpreter, such as starting
	///        and stopping it.
	virtual void init(EventsForKitPluginInterface const &eventsForKitPlugin
			, qReal::SystemEvents const &systemEvents
			, qReal::GraphicalModelAssistInterface &graphicalModel
			, qReal::LogicalModelAssistInterface &logicalModel
			, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
			, InterpreterControlInterface &interpreterControl)
	{
		Q_UNUSED(eventsForKitPlugin)
		Q_UNUSED(systemEvents)
		Q_UNUSED(graphicalModel)
		Q_UNUSED(logicalModel)
		Q_UNUSED(interpretersInterface)
		Q_UNUSED(interpreterControl)
	}

	/// An identifier of constructor kit. Kit plugins with same kitId are automaticly groupped
	/// together extending each other.
	virtual QString kitId() const = 0;

	/// String that will be displayed to users as the name of the kit.
	virtual QString friendlyKitName() const = 0;

	/// Returns a list of robot models supported by this kit plugin.
	virtual QList<robotModel::RobotModelInterface *> robotModels() = 0;

	/// Returns a blocks factory for a given robot model. Nullptr can be returned and means that no factory is provided
	/// by this plugin. Ownership must be transfered.
	virtual blocksBase::BlocksFactoryInterface *blocksFactoryFor(robotModel::RobotModelInterface const *model) = 0;

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
	virtual QWidget *quickPreferencesFor(robotModel::RobotModelInterface const &model)
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
	virtual QIcon iconForFastSelector(robotModel::RobotModelInterface const &robotModel) const = 0;

	/// Sensor configuration provider object for this plugin that needs to be registered in providers network.
	/// Can be a root of plugin-specific providers network. Null, if there is no need for sensors configuration.
	/// Does not transfer ownership.
	virtual interpreterBase::DevicesConfigurationProvider * devicesConfigurationProvider()
	{
		return nullptr;
	}
};

}

Q_DECLARE_INTERFACE(interpreterBase::KitPluginInterface, "ru.spbsu.math.QReal.KitPluginInterface/1")

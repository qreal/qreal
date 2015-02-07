#pragma once

#include <interpreterBase/devicesConfigurationProvider.h>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/plugins/toolPluginInterface/systemEvents.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>

namespace interpreterCore {

/// A class that stores device configuration information in a registry and is responsible for synchronizing this
/// information between various places where it can be changed.
/// General architecture for device settings management is this: everybody who has device configuration controls
/// or can alter device configurtion (2d model, DevicesConfigurationWidget, preferences page, actual robot model),
/// or contains them, inherits from DevicesConfigurationProvider and connects to other providers and eventually to
/// DevicesConfigurationManager object (by calling connectDevicesConfigurationProvider method).
///
/// Then any devicesConfigurationChanged calls are propagated through resulting network of interconnected
/// DevicesConfigurationProvider-s so they are kept in sync with each other.
/// DevicesConfigurationManager is a center of this network and is the one place which saves sensor settings
/// in a registry.
class DevicesConfigurationManager : public interpreterBase::DevicesConfigurationProvider
{
public:
	/// Constructor.
	/// @param graphicalModelAssistInterface - graphical model, needed to save settings as property of current diagram.
	/// @param logicalModelAssistInterface - logical model, also needed to save settings as property of current diagram.
	/// @param mainWindowInterpretersInterface - needed to get current active diagram
	/// @param systemEvents - provides notification about changing of active editor tab.
	DevicesConfigurationManager(qReal::GraphicalModelAssistInterface &graphicalModelAssistInterface
			, qReal::LogicalModelAssistInterface &logicalModelAssistInterface
			, qReal::gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface
			, qReal::SystemEvents &systemEvents
			);

private:
	void onDeviceConfigurationChanged(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &sensor
			, Reason reason) override;

	/// Reacts to changing of editor tab.
	/// @param graphicalRootId - id of a root diagram of newly selected tab.
	void onActiveTabChanged(qReal::Id const &graphicalRootId);

	/// Serializes current sensors configuration into inner string representation.
	QString save() const;

	/// Parses given sensors configuration serialized by save() method and broadcasts it to all connected providers.
	void load(QString const &configuration);

	qReal::GraphicalModelAssistInterface &mGraphicalModelAssistInterface;
	qReal::LogicalModelAssistInterface &mLogicalModelAssistInterface;
	qReal::gui::MainWindowInterpretersInterface &mMainWindowInterpretersInterface;
};

}

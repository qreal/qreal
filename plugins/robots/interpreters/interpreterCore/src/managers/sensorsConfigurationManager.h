#pragma once

#include <interpreterBase/sensorsConfigurationProvider.h>

#include <qrgui/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h>
#include <qrgui/toolPluginInterface/systemEvents.h>
#include <qrgui/mainwindow/mainWindowInterpretersInterface.h>

namespace interpreterCore {

/// A class that stores sensor configuration information in a registry and is responsible for synchronizing this
/// information between various places where it can be changed.
/// General architecture for sensors settings management is this: everybody who has sensor configuration controls
/// (2d model, SensorsConfigurationWidget, preferences page), or contains them, inherits from
/// SensorsConfigurationProvider and connects to other providers and eventually to SensorsSettingsManager object
/// (by calling connectSensorsConfigurationProvider method).
///
/// Then any sensorConfigurationChanged calls are propagated through resulting network of interconnected
/// SensorsConfigurationProvider-s so they are kept in sync with each other.
/// SensorsConfigurationManager is a center of this network and is the one place which saves sensor settings
/// in a registry.
///
/// Initialization is also centralized, SensorsSettingsManager::refresh() shall be called after all clients are
/// connected and this will initialize everything.
class SensorsConfigurationManager : public interpreterBase::SensorsConfigurationProvider
{
public:
	SensorsConfigurationManager(qReal::GraphicalModelAssistInterface &graphicalModelAssistInterface
			, qReal::LogicalModelAssistInterface &logicalModelAssistInterface
			, qReal::gui::MainWindowInterpretersInterface &mainWindowInterpretersInterface
			, qReal::SystemEventsInterface &systemEvents
			);

private:
	void onSensorConfigurationChanged(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &sensor) override;

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

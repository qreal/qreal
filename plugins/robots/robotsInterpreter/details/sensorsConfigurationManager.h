#pragma once

#include "sensorConstants.h"
#include "details/sensorsConfigurationProvider.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

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
class SensorsConfigurationManager : public SensorsConfigurationProvider
{
public:
	/// Sends sensorConfigurationChanged on all ports taking sensor types from registry to all clients
	/// to initialize them.
	void refresh();

private:
	static QString portToSettingsKey(qReal::interpreters::robots::enums::inputPort::InputPortEnum port);

	void onSensorConfigurationChanged(
			qReal::interpreters::robots::enums::inputPort::InputPortEnum port
			, qReal::interpreters::robots::enums::sensorType::SensorTypeEnum type
			) override;
};

}
}
}
}

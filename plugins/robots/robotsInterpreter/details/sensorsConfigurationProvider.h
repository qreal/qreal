#pragma once

#include "sensorConstants.h"

#include <QtCore/QList>
#include <QtCore/QVector>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

/// Mixin abstract class that shall be inherited by anyone who wants to change sensor configuration
/// or keep in sync with various places where sensor configuration can be changed.
class SensorsConfigurationProvider
{
public:
	/// Constructor.
	/// @param name - name of an instance of provider, which can be used in debug output.
	SensorsConfigurationProvider(QString const &name);

	virtual ~SensorsConfigurationProvider();

	/// Links other sensor configuration provider to this one, so when our calls
	/// sensorConfigurationChanged, onSensorConfigurationChanged will be called in other.
	void connectSensorsConfigurationProvider(SensorsConfigurationProvider * const otherProvider);

protected:
	/// Shall be called by descendants when sensor configuration is changed. Change is propagated automatically
	/// through entire graph of connected providers.
	/// @param port - input port on which sensor has changed.
	/// @param type - new type of a sensor on a given port.
	void sensorConfigurationChanged(
			qReal::interpreters::robots::enums::inputPort::InputPortEnum port
			, qReal::interpreters::robots::enums::sensorType::SensorTypeEnum type
			);

	/// Calls onSensorConfigurationChanged on every port, so provider can synchronize its internal data with sensors
	/// configuration if it was not able to do so for some reason.
	void refreshSensorsConfiguration();

private:
	/// Must be implemented in descendants to react to sensor configuration changes and refresh their internal data.
	/// Symmetric to sensorConfigurationChanged. Default implementation does nothing.
	virtual void onSensorConfigurationChanged(
			qReal::interpreters::robots::enums::inputPort::InputPortEnum port
			, qReal::interpreters::robots::enums::sensorType::SensorTypeEnum type
			);

	QList<SensorsConfigurationProvider *> mConnectedProviders;  // Doesn't have ownership.

	/// Redundant current sensor configuration to keep track of loops in provider network: if configuration is not
	/// changed by incoming message, it is not broadcasted.
	QVector<qReal::interpreters::robots::enums::sensorType::SensorTypeEnum> mCurrentConfiguration;

	/// Name of the provider, which can be used in debug output.
	QString mName;
};

}
}
}
}

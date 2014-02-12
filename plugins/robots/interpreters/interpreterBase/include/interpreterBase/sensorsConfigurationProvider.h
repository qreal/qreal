#pragma once

#include "robotModel/portInfo.h"
#include "robotModel/pluggableDeviceInfo.h"

#include <QtCore/QMap>

namespace interpreterBase {

/// Mixin abstract class that shall be inherited by anyone who wants to change sensor configuration
/// or keep in sync with various places where sensor configuration can be changed.
///
/// @todo Make all protected fields private. There is no public or protected fields, there is only private fields.
class ROBOTS_INTERPRETER_BASE_EXPORT SensorsConfigurationProvider
{
public:
	/// Constructor.
	/// @param name - name of an instance of provider, which can be used in debug output.
	explicit SensorsConfigurationProvider(QString const &name = QString());

	virtual ~SensorsConfigurationProvider();

	/// Links other sensor configuration provider to this one, so when our calls
	/// sensorConfigurationChanged, onSensorConfigurationChanged will be called in other.
	void connectSensorsConfigurationProvider(SensorsConfigurationProvider * const otherProvider);

protected:
	/// Shall be called by descendants when sensor configuration is changed. Change is propagated automatically
	/// through entire graph of connected providers.
	/// @param port - input port on which sensor has changed.
	/// @param type - new type of a sensor on a given port.
	void sensorConfigurationChanged(QString const &robotModel
			, robotModel::PortInfo const &port
			, robotModel::PluggableDeviceInfo const &sensor);

	/// Must be implemented in descendants to react to sensor configuration changes and refresh their internal data.
	/// Symmetric to sensorConfigurationChanged. Default implementation does nothing.
	virtual void onSensorConfigurationChanged(QString const &robotModel
			, robotModel::PortInfo const &port
			, robotModel::PluggableDeviceInfo const &sensor);

	/// Sets null devices to each known port of each known robot model
	void nullifyConfiguration();

	QList<SensorsConfigurationProvider *> mConnectedProviders;  // Doesn't have ownership.

	/// Redundant current sensor configuration to keep track of loops in provider network: if configuration is not
	/// changed by incoming message, it is not broadcasted.
	/// Hash structure is robotModel -> port -> device.
	QMap<QString, QMap<robotModel::PortInfo, robotModel::PluggableDeviceInfo>> mCurrentConfiguration;

	/// Name of the provider, which can be used in debug output.
	QString mName;
};

}

#pragma once

#include "robotModel/portInfo.h"
#include "robotModel/deviceInfo.h"

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

	/// Links other sensor configuration provider to this one, so when our calls sensorConfigurationChanged,
	/// onSensorConfigurationChanged will be called in other.
	/// Does nothing if otherProvider is null.
	/// Does not transfer ownership.
	void connectSensorsConfigurationProvider(SensorsConfigurationProvider * const otherProvider);

protected:
	/// Shall be called by descendants when device configuration is changed. Change is propagated automatically
	/// through entire graph of connected providers.
	/// @param port - input port on which device has changed.
	/// @param device - new type of a device on a given port.
	void sensorConfigurationChanged(QString const &robotModel
			, robotModel::PortInfo const &port
			, robotModel::DeviceInfo const &device);

	/// Must be implemented in descendants to react to sensor configuration changes and refresh their internal data.
	/// Symmetric to sensorConfigurationChanged. Default implementation does nothing.
	virtual void onSensorConfigurationChanged(QString const &robotModel
			, robotModel::PortInfo const &port
			, robotModel::DeviceInfo const &sensor);

	/// Sets null devices to each known port of each known robot model
	void nullifyConfiguration();

	/// Returns all models present in current configuration.
	QStringList configuredModels() const;

	/// Returns a list of ports with configured devices for given model.
	/// @todo Not sure that it is needed, we can ask model instead. Anyway, there are non-configurable ports and other
	///       stuff that only model knows.
	QList<robotModel::PortInfo> configuredPorts(QString const &modelName) const;

	/// Returns device configured on a given port in given model, or null device, if device on that port
	/// is not configured.
	robotModel::DeviceInfo currentConfiguration(QString const &modelName, robotModel::PortInfo const &port) const;

private:
	/// Redundant current sensor configuration.
	/// If configuration is not changed by incoming message, it is not broadcasted.
	/// Hash structure is robotModel -> port -> device.
	QMap<QString, QMap<robotModel::PortInfo, robotModel::DeviceInfo>> mCurrentConfiguration;

	/// Name of the provider, which can be used in debug output.
	QString mName;

	/// A list of providers connected to this one, to send notifications about configuration changes.
	QList<SensorsConfigurationProvider *> mConnectedProviders;  // Doesn't have ownership.
};

}

#pragma once

#include "robotModel/portInfo.h"
#include "robotModel/deviceInfo.h"

#include <QtCore/QMap>

namespace interpreterBase {

/// Mixin abstract class that shall be inherited by anyone who wants to change device configuration
/// or keep in sync with various places where device configuration can be changed. Configuration is stored
/// separately for each robot model, so, for example, changes made in NXT mode do not affect TRIK.
class ROBOTS_INTERPRETER_BASE_EXPORT DevicesConfigurationProvider
{
public:
	/// Constructor.
	/// @param name - name of an instance of provider, which can be used in debug output.
	explicit DevicesConfigurationProvider(QString const &name = QString());

	virtual ~DevicesConfigurationProvider();

	/// Links other device configuration provider to this one, so when our calls deviceConfigurationChanged,
	/// onDeviceConfigurationChanged will be called in other.
	/// Does nothing if otherProvider is null.
	/// Does not take ownership.
	void connectDevicesConfigurationProvider(DevicesConfigurationProvider * const otherProvider);
	void disconnectDevicesConfigurationProvider();

protected:
	/// Reason for configuration change.
	enum class Reason {
		/// User himself changed device by, for example, selecting sensor in settings window.
		userAction

		/// Configuration change occured during loading of diagram or save with model.
		, loading

		/// Configuration was changed by autoconfigurer, so everything shall work just as if it is done by user, but
		/// user shall be notified about it.
		, automaticConfiguration
	};

	/// Shall be called by descendants when device configuration is changed. Change is propagated automatically
	/// through entire graph of connected providers.
	/// @param robotModel - robot model name to which configuration change is applied.
	/// @param port - input port on which device has changed.
	/// @param device - new type of a device on a given port.
	/// @param reason - reason for configuration change. It allows different reactions to, for example, changes during
	///        save loading and to changes made by user or autoconfigurer.
	void deviceConfigurationChanged(QString const &robotModel
			, robotModel::PortInfo const &port
			, robotModel::DeviceInfo const &device
			, Reason reason);

	/// Must be implemented in descendants to react to device configuration changes and refresh their internal data.
	/// Symmetric to deviceConfigurationChanged. Default implementation does nothing.
	virtual void onDeviceConfigurationChanged(QString const &robotModel
			, robotModel::PortInfo const &port
			, robotModel::DeviceInfo const &device
			, Reason reason);

	/// Sets null devices to each known port of each known robot model.
	void clearConfiguration(Reason reason);

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
	void disconnectDevicesConfigurationProvider(DevicesConfigurationProvider * const provider);

	/// Redundant current devices configuration.
	/// If configuration is not changed by incoming message, it is not broadcasted.
	/// Hash structure is robotModel -> port -> device.
	QMap<QString, QMap<robotModel::PortInfo, robotModel::DeviceInfo>> mCurrentConfiguration;

	/// Name of the provider, which can be used in debug output.
	QString mName;

	/// A list of providers connected to this one, to send notifications about configuration changes.
	QList<DevicesConfigurationProvider *> mConnectedProviders;  // Doesn't have ownership.
};

}

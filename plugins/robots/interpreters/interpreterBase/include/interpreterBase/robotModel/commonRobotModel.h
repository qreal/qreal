#pragma once

#include <QtCore/QScopedPointer>

#include "interpreterBase/robotModel/robotModelInterface.h"
#include "interpreterBase/robotModel/configuration.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

/// Base implementation of all kit models. Provides common functionality and default implementations for
/// RobotModelInterface methods.
class ROBOTS_INTERPRETER_BASE_EXPORT CommonRobotModel : public RobotModelInterface
{
	Q_OBJECT

public:
	CommonRobotModel();
	~CommonRobotModel() override;

	/// Defines model initialization sequence.
	void init() final;

	/// Shall be reimplemented in models which need connection to a robot. Default implementation just
	/// emits connected().
	void connectToRobot() override;

	/// Shall be implemented in descendants to stop all robot devices. Default implementation does nothing.
	void stopRobot() override;

	/// Shall be reimplemented in models that need sometihng done to disconnect from robot. Default implementation just
	/// emits disconnected().
	void disconnectFromRobot() override;

	ConnectionState connectionState() const final;

	ConfigurationInterface const &configuration() const final;

	QList<PortInfo> availablePorts() const final;

	/// @todo Make default implementation use allowedDevices() to determine if there is actually a choise.
	QList<PortInfo> configurablePorts() const override;

	QList<DeviceInfo> allowedDevices(PortInfo const &port) const final;

	void configureDevice(PortInfo const &port, DeviceInfo const &deviceInfo) final;

	void applyConfiguration() final;

	/// Shall be reimplemented to provide base device classes through which devices are convertable (see description
	/// of that method in RobotModelInterface for more details). Default implementation returns empty list, so
	/// no devices are convertable.
	QList<DeviceInfo> convertibleBases() const override;

	/// Returns real-time timer implementation. Must be redefined for 2D model-based robot models.
	utils::AbstractTimer *produceTimer() override;

public slots:
	/// Shall be reimplemented to update settings when user changes something on settings page. Default implementation
	/// does nothing.
	virtual void rereadSettings();

protected:
	/// Adds to a model list of devices that can be connected to given port.
	void addAllowedConnection(PortInfo const &port, QList<DeviceInfo> const &devices);

	/// Configuration that can be changed by descendants to register their devices.
	ConfigurationInterface &mutableConfiguration();

	/// Configures devices that can be configured automaticly (non-user configured, such as display and speakers)
	virtual void configureKnownDevices();

private slots:
	void onConnected(bool success);

	void onDisconnected();

private:
	/// Device factory. Shall be reimplemented to create concrete devices on given port by given device info.
	/// If device is unknown to a model, shall throw exception, as a list of available devices is provided by model
	/// itself by allowedDevices() method.
	virtual robotParts::Device * createDevice(PortInfo const &port, DeviceInfo const &deviceInfo);

	/// Shows which types of devices can be connected to which ports.
	/// @todo Add a notion of direction.
	QHash<PortInfo, QList<DeviceInfo>> mAllowedConnections;

	/// Devices configuration.
	Configuration mConfiguration;

	/// Model connection state.
	ConnectionState mState;

	/// True, if someone called applyConfiguration() but model was disconnected, so configuration will be applied
	/// when model becomes connected.
	bool mConfigurationPostponed;
};

}
}

#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "interpreterBase/robotModel/portInfo.h"
#include "interpreterBase/robotModel/deviceInfo.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Anything that can be plugged into a port on a robot. Bound to a port at creation time and shall be configured
/// before it becomes ready to work. Base class for all engines/sensors/displays and so on.
/// Each concrete device should have Q_CLASSINFO macroses for friendlyName and device direction.
class ROBOTS_INTERPRETER_BASE_EXPORT Device : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param port - port on which this device shall be configured.
	Device(DeviceInfo const &info, PortInfo const &port);

	virtual ~Device();

	/// Returns port on which this device is or shall be configured.
	PortInfo const &port() const;

	/// Returns device type for non-abstract (physical) device.
	virtual DeviceInfo const &deviceInfo() const;

	/// Returns true if device is ready for work (i.e. configured).
	bool ready() const;

	/// Configure device.
	void configure();

signals:
	/// Emitted when device finished its configuration, successfully or by failure. Note that configuration can be
	/// synchronous or asynchronous for various devices (configure() can be in a stack when this signal is emitted).
	/// Shall not be emitted directly from descendants, use configurationCompleted() instead due to timeout handling
	/// performed by Device class.
	void configured(bool success);

protected:
	/// Perform actual configuration of a device. Shall be implemented for concrete devices.
	virtual void doConfiguration() = 0;

	/// Concrete devices shall call this method when they finish their configuration.
	void configurationCompleted(bool success);

private slots:
	/// Called when configuration takes more than timeout time. Note that if configuration is done synchronously,
	/// timer signal will never be emitted and this slot will not be called.
	void configurationTimerTimeoutSlot();

private:
	DeviceInfo const &mInfo;
	PortInfo mPort;
	QTimer mConfigurationTimeoutTimer;
};

}
}
}

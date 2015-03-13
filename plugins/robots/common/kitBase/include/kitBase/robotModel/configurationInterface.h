#pragma once

#include <QtCore/QObject>
#include <QtCore/QHash>

#include "kitBase/robotModel/portInfo.h"
#include "kitBase/robotModel/robotParts/device.h"

#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {

/// Provides current configuration (devices connected to ports and ready to work) and ability to manage it.
class ROBOTS_KIT_BASE_EXPORT ConfigurationInterface : public QObject
{
	Q_OBJECT

public:
	~ConfigurationInterface() override {}

	/// Adds device to pending configuration. Actual configuring of a device on a robot is done in applyConfiguration().
	/// @param device - device to be added to configuration. Transfers ownership.
	virtual void configureDevice(robotParts::Device * const device) = 0;

	/// Uploads current configuration to a robot.
	/// Guaranteed to emit allDevicesConfigured() if all devices respond about their configuration status.
	virtual void applyConfiguration() = 0;

	/// Returns all configured devices. Allows to enumerate configured devices.
	/// Does not transfer ownership of devices.
	virtual QList<robotParts::Device *> devices() const = 0;

	/// Returns configured device on a given port or nullptr if no device is configured there.
	/// Does not transfer ownership.
	virtual robotParts::Device *device(const PortInfo &port) const = 0;

	/// Removes device on a given port from configuration.
	virtual void clearDevice(const PortInfo &port) = 0;
};

}
}

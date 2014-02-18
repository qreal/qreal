#pragma once

#include <QtCore/QObject>
#include <QtCore/QHash>

#include "interpreterBase/robotModel/portInfo.h"
#include "interpreterBase/robotModel/robotParts/device.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

class ROBOTS_INTERPRETER_BASE_EXPORT ConfigurationInterface : public QObject
{
	Q_OBJECT

public:
	enum PortDirection {
		defaultDirection
		, input
		, output
	};

	virtual ~ConfigurationInterface() {}

	/// Adds device to pending configuration. Actual configuring of a device on a robot is done in applyConfiguration().
	/// @param device - device to be added to configuration. Transfers ownership.
	virtual void configureDevice(robotParts::Device * const device) = 0;

	/// Guaranteed to emit allDevicesConfigured() if all devices respond about their configuration status.
	virtual void applyConfiguration() = 0;

	/// Returns all configured devices with given port direction. Allows to enumerate configured devices.
	virtual QList<robotParts::Device *> devices(
			PortDirection direction = defaultDirection) const = 0;

	/// Returns configured device on a given port or nullptr if no device is configured there.
	virtual robotParts::Device *device(
			PortInfo const &port
			, PortDirection direction = defaultDirection) const = 0;

	/// \todo Implement some convenience methods that cast generic Device to desired sensor/motor type.

	virtual void clearDevice(PortInfo const &port) = 0;
};

}
}

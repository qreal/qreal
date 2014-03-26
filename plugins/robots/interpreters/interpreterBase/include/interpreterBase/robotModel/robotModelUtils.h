#pragma once

#include "robotModelInterface.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

/// Provides some useful methods for working with robot models
class ROBOTS_INTERPRETER_BASE_EXPORT RobotModelUtils
{
public:
	/// Returns a pointer to a device if it is configured on the given port and ready to work
	template<typename T>
	static T *findDevice(RobotModelInterface const &robotModel, PortInfo const &port)
	{
		DeviceInfo const info = DeviceInfo::create<T>();
		QList<DeviceInfo> const possibleDevices = robotModel.allowedDevices(port);
		ConfigurationInterface::PortDirection const direction = possibleDevices.count() == 2
				? (possibleDevices[0].direction() != possibleDevices[1].direction()
						? convertDirection(info.direction())
						: ConfigurationInterface::defaultDirection)
				: ConfigurationInterface::defaultDirection;

		return dynamic_cast<T *>(robotModel.configuration().device(port, direction));
	}

	/// Returns a pointer to a device if it is configured on the given port string representation and ready to work
	template<typename T>
	static T *findDevice(RobotModelInterface const &robotModel, QString const &port)
	{
		PortInfo const portInfo = findPort(robotModel, port);
		return portInfo.isValid() ? findDevice<T>(robotModel, portInfo) : nullptr;
	}

	/// Performs search over all robot ports and returns the first found one with the given string representation.
	/// If no suitable ports found invalid instance is returned.
	static PortInfo findPort(RobotModelInterface const &robotModel, QString const &stringRepresentation);

private:
	static ConfigurationInterface::PortDirection convertDirection(DeviceInfo::Direction direction);
};

}
}

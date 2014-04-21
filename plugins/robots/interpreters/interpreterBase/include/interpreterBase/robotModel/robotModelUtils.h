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
		return dynamic_cast<T *>(robotModel.configuration().device(port));
	}

	/// Returns a pointer to a device if it is configured on the given port string representation and ready to work
	template<typename T>
	static T *findDevice(RobotModelInterface const &robotModel, QString const &port)
	{
		PortInfo const portInfo = findPort(robotModel, port, DeviceInfo::create<T>().direction());
		return portInfo.isValid() ? findDevice<T>(robotModel, portInfo) : nullptr;
	}

	/// Performs search over all robot ports and returns the first found one with the given string representation.
	/// If no suitable ports found invalid instance is returned.
	static PortInfo findPort(RobotModelInterface const &robotModel, QString const &name, Direction direction);

	/// Converts string representation of devices configuration into a number of maps:
	/// robot model is mapped into map of its ports to devices configured on them.
	static QMap<QString, QMap<PortInfo, DeviceInfo>> deserialize(QString const &configuration);
};

}
}

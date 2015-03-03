#pragma once

#include "robotModelInterface.h"
#include "kitBase/kitPluginInterface.h"
#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {

/// Provides some useful methods for working with robot models
class ROBOTS_KIT_BASE_EXPORT RobotModelUtils
{
public:
	/// Returns a pointer to a device if it is configured on the given port and ready to work.
	template<typename T>
	static T *findDevice(const RobotModelInterface &robotModel, const PortInfo &port)
	{
		return dynamic_cast<T *>(robotModel.configuration().device(port));
	}

	/// Returns a pointer to a device if it is configured on the given port string representation and ready to work.
	template<typename T>
	static T *findDevice(const RobotModelInterface &robotModel, const QString &port)
	{
		const PortInfo portInfo = findPort(robotModel, port, DeviceInfo::create<T>().direction());
		return portInfo.isValid() ? findDevice<T>(robotModel, portInfo) : nullptr;
	}

	/// Performs search over all robot ports and returns the first found one with the given string representation.
	/// If no suitable ports found invalid instance is returned.
	static PortInfo findPort(const RobotModelInterface &robotModel, const QString &name, Direction direction);

	/// Converts string representation of devices configuration into a number of maps:
	/// robot model is mapped into map of its ports to devices configured on them.
	static QMap<QString, QMap<PortInfo, DeviceInfo>> deserialize(const QString &configuration);

	/// Returns lastly selected robot model for the given kits set with the same id.
	/// If user didn`t select robot model yet then robot model specified in kit as default one is returned.
	/// If no default robot model specified in kit then the first robot model is returned.
	/// Finally if kit does not contain robot models at all nullptr is returned.
	static RobotModelInterface *selectedRobotModelFor(QList<KitPluginInterface *> const &kits);

	/// Sorts robot models in descending order of their priority.
	/// @see RobotModelInterface::priority().
	static void sortRobotModels(QList<RobotModelInterface *> &robotModels);
};

}
}

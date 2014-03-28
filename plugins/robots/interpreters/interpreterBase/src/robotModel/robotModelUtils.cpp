#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace interpreterBase::robotModel;

PortInfo RobotModelUtils::findPort(RobotModelInterface const &robotModel, QString const &stringRepresentation)
{
	for (interpreterBase::robotModel::PortInfo const &portInfo : robotModel.availablePorts()) {
		if (portInfo.name() == stringRepresentation || portInfo.nameAliases().contains(stringRepresentation)) {
			return portInfo;
		}
	}

	return PortInfo();
}

ConfigurationInterface::PortDirection RobotModelUtils::convertDirection(DeviceInfo::Direction direction)
{
	switch (direction) {
	case DeviceInfo::input:
		return ConfigurationInterface::input;
	case DeviceInfo::output:
		return ConfigurationInterface::output;
	default:
		return ConfigurationInterface::defaultDirection;
	}
}

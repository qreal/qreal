#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace interpreterBase::robotModel;

PortInfo RobotModelUtils::findPort(RobotModelInterface const &robotModel, QString const &name, Direction direction)
{
	for (interpreterBase::robotModel::PortInfo const &portInfo : robotModel.availablePorts()) {
		if ((portInfo.name() == name || portInfo.nameAliases().contains(name)) && portInfo.direction() == direction) {
			return portInfo;
		}
	}

	return PortInfo();
}

#pragma once

#include <interpreterBase/robotModel/robotParts/colorSensorPassive.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "implementations/colorSensorImpl.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorPassive : public interpreterBase::robotModel::robotParts::ColorSensorPassive
{
	Q_OBJECT

public:
	ColorSensorPassive(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void read() override;

protected:
	void doConfiguration() override;

private:
	ColorSensorImpl mImpl;
};

}
}
}
}

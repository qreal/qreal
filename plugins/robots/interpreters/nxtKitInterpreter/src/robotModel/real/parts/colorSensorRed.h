#pragma once

#include <interpreterBase/robotModel/robotParts/colorSensorRed.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "implementations/colorSensorImpl.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorRed : public interpreterBase::robotModel::robotParts::ColorSensorRed
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Color sensor (red)"))

public:
	ColorSensorRed(interpreterBase::robotModel::DeviceInfo const &info
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

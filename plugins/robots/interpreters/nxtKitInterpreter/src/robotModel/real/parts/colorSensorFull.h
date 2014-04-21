#pragma once

#include <interpreterBase/robotModel/robotParts/colorSensorFull.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "implementations/colorSensorImpl.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorFull : public interpreterBase::robotModel::robotParts::ColorSensorFull
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Color sensor (full)"))

public:
	ColorSensorFull(interpreterBase::robotModel::DeviceInfo const &info
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

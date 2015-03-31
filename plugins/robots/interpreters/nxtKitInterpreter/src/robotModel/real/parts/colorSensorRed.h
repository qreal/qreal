#pragma once

#include <kitBase/robotModel/robotParts/colorSensorRed.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "implementations/colorSensorImpl.h"

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorRed : public kitBase::robotModel::robotParts::ColorSensorRed
{
	Q_OBJECT

public:
	ColorSensorRed(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
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

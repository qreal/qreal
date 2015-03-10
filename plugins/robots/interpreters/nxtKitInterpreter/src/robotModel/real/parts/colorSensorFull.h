#pragma once

#include <kitBase/robotModel/robotParts/colorSensorFull.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "implementations/colorSensorImpl.h"

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorFull : public kitBase::robotModel::robotParts::ColorSensorFull
{
	Q_OBJECT

public:
	ColorSensorFull(const kitBase::robotModel::DeviceInfo &info
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

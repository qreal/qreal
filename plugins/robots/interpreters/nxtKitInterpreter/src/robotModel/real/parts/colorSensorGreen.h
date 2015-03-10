#pragma once

#include <kitBase/robotModel/robotParts/colorSensorGreen.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "implementations/colorSensorImpl.h"

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorGreen : public kitBase::robotModel::robotParts::ColorSensorGreen
{
	Q_OBJECT

public:
	ColorSensorGreen(const kitBase::robotModel::DeviceInfo &info
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

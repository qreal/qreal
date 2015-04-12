#pragma once

#include <kitBase/robotModel/robotParts/colorSensorBlue.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "implementations/colorSensorImpl.h"

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorBlue : public kitBase::robotModel::robotParts::ColorSensorBlue
{
	Q_OBJECT

public:
	ColorSensorBlue(const kitBase::robotModel::DeviceInfo &info
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

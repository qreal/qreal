#pragma once

#include <kitBase/robotModel/robotParts/touchSensor.h>
#include "nxtInputDevice.h"

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class TouchSensor : public kitBase::robotModel::robotParts::TouchSensor
{
	Q_OBJECT

public:
	TouchSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void read() override;
	void doConfiguration() override;

private slots:
	void sensorSpecificProcessResponse(const QByteArray &reading);

private:
	NxtInputDevice mImplementation;
};

}
}
}
}

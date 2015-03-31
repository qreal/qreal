#pragma once

#include <kitBase/robotModel/robotParts/gyroscopeSensor.h>
#include "nxtInputDevice.h"

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class GyroscopeSensor : public kitBase::robotModel::robotParts::GyroscopeSensor
{
	Q_OBJECT

public:
	GyroscopeSensor(const kitBase::robotModel::DeviceInfo &info
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

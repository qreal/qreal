#pragma once

#include <kitBase/robotModel/robotParts/lightSensor.h>
#include "nxtInputDevice.h"

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class LightSensor : public kitBase::robotModel::robotParts::LightSensor
{
	Q_OBJECT

public:
	LightSensor(const kitBase::robotModel::DeviceInfo &info
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

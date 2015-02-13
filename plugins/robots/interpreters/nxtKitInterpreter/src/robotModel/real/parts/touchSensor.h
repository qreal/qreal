#pragma once

#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include "nxtInputDevice.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class TouchSensor : public interpreterBase::robotModel::robotParts::TouchSensor
{
	Q_OBJECT

public:
	TouchSensor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
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

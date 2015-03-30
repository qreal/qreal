#pragma once

#include <kitBase/robotModel/robotParts/soundSensor.h>
#include "nxtInputDevice.h"

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class SoundSensor : public kitBase::robotModel::robotParts::SoundSensor
{
	Q_OBJECT

public:
	SoundSensor(const kitBase::robotModel::DeviceInfo &info
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

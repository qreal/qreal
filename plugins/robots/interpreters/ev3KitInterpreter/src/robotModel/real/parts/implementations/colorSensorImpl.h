#pragma once

#include <kitBase/robotModel/robotParts/colorSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "src/commandConstants.h"
#include "src/robotModel/real/parts/ev3InputDevice.h"

namespace ev3 {
namespace robotModel {
namespace real {
namespace parts {

class ColorSensorImpl : public QObject
{
	Q_OBJECT

public:
	ColorSensorImpl(const kitBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator
			, SensorTypeEnum lowLevelType);

	void read();

signals:
	void newData(int reading);

private:
	Ev3InputDevice mImplementation;
	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
	SensorTypeEnum mLowLevelType;
};

}
}
}
}

#pragma once

#include <QtCore/QByteArray>

#include <interpreterBase/robotModel/robotParts/abstractSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "commandConstants.h"

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Ev3InputDevice : public QObject
{
	Q_OBJECT

public:

	Ev3InputDevice(utils::robotCommunication::RobotCommunicator &robotCommunicator
			, const interpreterBase::robotModel::PortInfo &port
			, SensorTypeEnum lowLevelSensorType
			, INPUT_DEVICE_SUBCODE sensorMode);

	void send(QByteArray command, const unsigned responseSize, QByteArray reading);

	/// Returns a value of port that can be used as corresponding byte in request packages.
	char lowLevelPort() const;

private:

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
	char mLowLevelPort;
	SensorTypeEnum mLowLevelSensorType;
	INPUT_DEVICE_SUBCODE mSensorMode;
};

}
}
}
}

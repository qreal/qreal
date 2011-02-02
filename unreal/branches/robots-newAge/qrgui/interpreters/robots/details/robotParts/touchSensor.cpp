#include "touchSensor.h"

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

TouchSensor::TouchSensor(RobotCommunicationInterface *robotCommunicationInterface
		, lowLevelInputPort::InputPortEnum const &port)
	: Sensor(robotCommunicationInterface, lowLevelSensorType::SWITCH, sensorMode::BOOLEANMODE, port)
{
}

void TouchSensor::sensorSpecificProcessResponse(QByteArray const &reading)
{
	mState = idle;
	if (reading[4] == 0 && reading[14] == 1)  // Sensor is pressed.
		emit response(1);
	else
		emit response(0);
}

#include "touchSensor.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

TouchSensor::TouchSensor(RobotCommunicationInterface *robotCommunicationInterface
		, lowLevelInputPort::InputPortEnum const &port)
	: Sensor(robotCommunicationInterface, lowLevelSensorType::SWITCH, sensorMode::BOOLEANMODE, port)
{
}

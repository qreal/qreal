#include "touchSensor.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

TouchSensor::TouchSensor(RobotCommunicationInterface *robotCommunicationInterface, int port)
	: Sensor(robotCommunicationInterface, SensorType::touchBoolean, port)
{
}

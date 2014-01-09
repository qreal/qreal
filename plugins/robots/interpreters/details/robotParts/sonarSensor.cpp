#include "sonarSensor.h"

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

SonarSensor::SonarSensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl, robots::enums::inputPort::InputPortEnum const port)
	: Sensor(sensorImpl,port)
{
}

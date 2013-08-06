#include "gyroscopeSensor.h"

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

GyroscopeSensor::GyroscopeSensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl
		, robots::enums::inputPort::InputPortEnum const port)
	: Sensor(sensorImpl, port)
{
}

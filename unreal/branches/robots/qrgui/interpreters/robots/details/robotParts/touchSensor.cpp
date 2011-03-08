#include "touchSensor.h"

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

TouchSensor::TouchSensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl,inputPort::InputPortEnum const &port)
	: Sensor(sensorImpl, port)
{
}

#include "lightSensor.h"

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

LightSensor::LightSensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl, inputPort::InputPortEnum const &port)
	: Sensor(sensorImpl,port)
{
}


#include "accelerometerSensor.h"

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

AccelerometerSensor::AccelerometerSensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl, inputPort::InputPortEnum const port)
	: Sensor(sensorImpl, port)
{
}

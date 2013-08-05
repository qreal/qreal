#include "soundSensor.h"

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

SoundSensor::SoundSensor(robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl
		, robots::enums::inputPort::InputPortEnum const port)
	: Sensor(sensorImpl, port)
{
}

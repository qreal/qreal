#include "colorSensor.h"

using namespace qReal::interpreters::robots;
using namespace details::robotParts;

ColorSensor::ColorSensor(
		robotImplementations::sensorImplementations::AbstractSensorImplementation *sensorImpl
		, robots::enums::inputPort::InputPortEnum const port
		)
		: Sensor(sensorImpl, port)
{
}

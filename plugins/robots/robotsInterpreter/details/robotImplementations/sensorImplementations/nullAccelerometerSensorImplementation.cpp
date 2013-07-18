#include "nullAccelerometerSensorImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullAccelerometerSensorImplementation::NullAccelerometerSensorImplementation(robots::enums::inputPort::InputPortEnum const port)
	: NullSensorImplementation(port, robots::enums::sensorType::accelerometer)
{
}

void NullAccelerometerSensorImplementation::timerTimeout()
{
	emit response(0);
}

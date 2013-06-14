#include "nullAccelerometerSensorImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullAccelerometerSensorImplementation::NullAccelerometerSensorImplementation(inputPort::InputPortEnum const port)
	: NullSensorImplementation(port, sensorType::accelerometer)
{
}

void NullAccelerometerSensorImplementation::timerTimeout()
{
	emit response(0);
}

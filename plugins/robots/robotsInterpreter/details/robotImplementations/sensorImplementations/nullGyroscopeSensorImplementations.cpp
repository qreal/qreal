#include "nullGyroscopeSensorImplementations.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullGyroscopeSensorImplementation::NullGyroscopeSensorImplementation(inputPort::InputPortEnum const port)
	: NullSensorImplementation(port, sensorType::gyroscope)
{
}

void NullGyroscopeSensorImplementation::timerTimeout()
{
	emit response(0);
}

#include "nullGyroscopeSensorImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullGyroscopeSensorImplementation::NullGyroscopeSensorImplementation(robots::enums::inputPort::InputPortEnum const port)
	: NullSensorImplementation(port, robots::enums::sensorType::gyroscope)
{
}

void NullGyroscopeSensorImplementation::timerTimeout()
{
	emit response(0);
}

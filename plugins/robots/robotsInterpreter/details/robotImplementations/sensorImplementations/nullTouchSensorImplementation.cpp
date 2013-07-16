#include "nullTouchSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullTouchSensorImplementation::NullTouchSensorImplementation(robots::enums::inputPort::InputPortEnum const port)
	: NullSensorImplementation(port, robots::enums::sensorType::touchBoolean)
{
}

void NullTouchSensorImplementation::timerTimeout()
{
	emit response(0);
}

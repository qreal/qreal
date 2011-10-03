#include "nullTouchSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullTouchSensorImplementation::NullTouchSensorImplementation(inputPort::InputPortEnum const &port)
	: NullSensorImplementation(port, sensorType::touchBoolean)
{
}

void NullTouchSensorImplementation::timerTimeout()
{
	emit response(0);
}

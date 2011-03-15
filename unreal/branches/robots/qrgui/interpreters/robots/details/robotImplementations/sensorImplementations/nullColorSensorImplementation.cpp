#include "nullColorSensorImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullColorSensorImplementation::NullColorSensorImplementation(inputPort::InputPortEnum const &port)
	: NullSensorImplementation(port)
{
}

void NullColorSensorImplementation::timerTimeout()
{
	emit response(0);
}

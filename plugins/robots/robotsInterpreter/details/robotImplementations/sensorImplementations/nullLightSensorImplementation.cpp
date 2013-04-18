#include "nullLightSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullLightSensorImplementation::NullLightSensorImplementation(inputPort::InputPortEnum const &port)
	: NullSensorImplementation(port, sensorType::light)
{
}

void NullLightSensorImplementation::timerTimeout()
{
	emit response(0);
}

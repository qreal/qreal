#include "nullLightSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullLightSensorImplementation::NullLightSensorImplementation(robots::enums::inputPort::InputPortEnum const port)
	: NullSensorImplementation(port, robots::enums::sensorType::light)
{
}

void NullLightSensorImplementation::timerTimeout()
{
	emit response(0);
}

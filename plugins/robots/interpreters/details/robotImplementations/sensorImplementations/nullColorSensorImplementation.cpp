#include "nullColorSensorImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullColorSensorImplementation::NullColorSensorImplementation(robots::enums::inputPort::InputPortEnum const port, robots::enums::sensorType::SensorTypeEnum const &sensorType)
	: NullSensorImplementation(port, sensorType)
{
}

void NullColorSensorImplementation::timerTimeout()
{
	emit response(0);
}

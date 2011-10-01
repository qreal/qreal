#include "nullColorSensorImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullColorSensorImplementation::NullColorSensorImplementation(inputPort::InputPortEnum const &port, sensorType::SensorTypeEnum const &sensorType)
	: NullSensorImplementation(port, sensorType)
{
}

void NullColorSensorImplementation::timerTimeout()
{
	emit response(0);
}

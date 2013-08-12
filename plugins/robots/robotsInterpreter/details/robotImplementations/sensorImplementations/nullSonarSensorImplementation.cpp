#include "nullSonarSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullSonarSensorImplementation::NullSonarSensorImplementation(robots::enums::inputPort::InputPortEnum const port)
	: NullSensorImplementation(port, robots::enums::sensorType::sonar)
{
}

void NullSonarSensorImplementation::timerTimeout()
{
	emit response(13);
}

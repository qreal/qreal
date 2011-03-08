#include "nullSonarSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullSonarSensorImplementation::NullSonarSensorImplementation(inputPort::InputPortEnum const &port)
	: NullSensorImplementation(port)
{
}

void NullSonarSensorImplementation::timerTimeout()
{
	emit response(13);
}

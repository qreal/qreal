#include "nullSoundSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullSoundSensorImplementation::NullSoundSensorImplementation(inputPort::InputPortEnum const &port)
	: NullSensorImplementation(port, sensorType::sound)
{
}

void NullSoundSensorImplementation::timerTimeout()
{
	emit response(0);
}


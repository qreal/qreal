#include "nullSoundSensorImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

NullSoundSensorImplementation::NullSoundSensorImplementation(robots::enums::inputPort::InputPortEnum const port)
	: NullSensorImplementation(port, robots::enums::sensorType::sound)
{
}

void NullSoundSensorImplementation::timerTimeout()
{
	emit response(0);
}


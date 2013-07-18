#include "unrealSoundSensorImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealSoundSensorImplementation::UnrealSoundSensorImplementation(robots::enums::inputPort::InputPortEnum const port, d2Model::D2RobotModel *d2Model)
	: UnrealSensorImplementation(port, d2Model, robots::enums::sensorType::sound)
{
}

void UnrealSoundSensorImplementation::read()
{
	emit response(0);
}

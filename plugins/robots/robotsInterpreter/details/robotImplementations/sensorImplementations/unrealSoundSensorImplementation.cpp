#include "unrealSoundSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealSoundSensorImplementation::UnrealSoundSensorImplementation(inputPort::InputPortEnum const &port, d2Model::D2RobotModel *d2Model)
	: UnrealSensorImplementation(port, d2Model, sensorType::sound)
{
}

void UnrealSoundSensorImplementation::read()
{
	Q_UNUSED(this);
}

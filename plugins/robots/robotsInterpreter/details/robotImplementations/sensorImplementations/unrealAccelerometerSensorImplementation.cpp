#include "unrealAccelerometerSensorImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealAccelerometerSensorImplementation::UnrealAccelerometerSensorImplementation(robots::enums::inputPort::InputPortEnum const port, d2Model::D2RobotModel *d2Model)
	: UnrealSensorImplementation(port, d2Model, robots::enums::sensorType::sound)
{
}

void UnrealAccelerometerSensorImplementation::read()
{
	emit response(0);
}

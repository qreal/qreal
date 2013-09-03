#include "unrealGyroscopeSensorImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealGyroscopeSensorImplementation::UnrealGyroscopeSensorImplementation(robots::enums::inputPort::InputPortEnum const port, d2Model::D2RobotModel *d2Model)
	: UnrealSensorImplementation(port, d2Model, robots::enums::sensorType::gyroscope)
{
}

void UnrealGyroscopeSensorImplementation::read()
{
	emit response(0);
}

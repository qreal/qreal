#include "unrealTouchSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealTouchSensorImplementation::UnrealTouchSensorImplementation(inputPort::InputPortEnum const &port, d2Model::D2RobotModel *d2Model)
	: UnrealSensorImplementation(port, d2Model)
{
}

void UnrealTouchSensorImplementation::read()
{
	emit response(0);
}

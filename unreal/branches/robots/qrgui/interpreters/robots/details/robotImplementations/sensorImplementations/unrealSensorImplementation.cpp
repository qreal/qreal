#include "unrealSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealSensorImplementation::UnrealSensorImplementation(inputPort::InputPortEnum const &port, d2Model::D2RobotModel *d2Model)
	: AbstractSensorImplementation(port)
	, mD2Model(d2Model)
{
}

void UnrealSensorImplementation::read()
{
}

#include "unrealTouchSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealTouchSensorImplementation::UnrealTouchSensorImplementation(inputPort::InputPortEnum const &port, d2Model::D2RobotModel *d2Model)
	: UnrealSensorImplementation(port, d2Model, sensorType::touchBoolean)
{
}

void UnrealTouchSensorImplementation::read()
{
	emit response(mD2Model->readTouchSensor(mPort));
}

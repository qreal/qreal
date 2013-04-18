#include "unrealLightSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealLightSensorImplementation::UnrealLightSensorImplementation(inputPort::InputPortEnum const &port, d2Model::D2RobotModel *d2Model)
	: UnrealSensorImplementation(port, d2Model, sensorType::light)
{
}

void UnrealLightSensorImplementation::read()
{
	emit response(mD2Model->readLightSensor(mPort));
}

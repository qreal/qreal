#include "unrealSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealSensorImplementation::UnrealSensorImplementation(inputPort::InputPortEnum const &port, d2Model::D2RobotModel *d2Model, sensorType::SensorTypeEnum const &sensorType)
	: AbstractSensorImplementation(port, sensorType)
	, mD2Model(d2Model)
{
}

void UnrealSensorImplementation::configure()
{
	emit configured();
}

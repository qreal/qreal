#include "unrealGyroscopeSensorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealGyroscopeSensorImplementation::UnrealGyroscopeSensorImplementation(inputPort::InputPortEnum const port, d2Model::D2RobotModel *d2Model)
	: UnrealSensorImplementation(port, d2Model, sensorType::gyroscope)
{
}

void UnrealGyroscopeSensorImplementation::read()
{

}

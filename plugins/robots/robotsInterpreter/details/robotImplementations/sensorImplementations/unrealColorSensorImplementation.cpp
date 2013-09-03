#include "unrealColorSensorImplementation.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealColorSensorImplementation::UnrealColorSensorImplementation(robots::enums::inputPort::InputPortEnum const port, d2Model::D2RobotModel *d2Model, robots::enums::sensorType::SensorTypeEnum const &sensorType)
	: UnrealSensorImplementation(port, d2Model, sensorType)
{
}

void UnrealColorSensorImplementation::read()
{
	emit response(mD2Model->readColorSensor(mPort));
}

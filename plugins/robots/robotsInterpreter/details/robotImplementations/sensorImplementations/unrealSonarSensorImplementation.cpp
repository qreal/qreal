#include "unrealSonarSensorImplementation.h"
#include <QtCore/QDebug>
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealSonarSensorImplementation::UnrealSonarSensorImplementation(robots::enums::inputPort::InputPortEnum const port, d2Model::D2RobotModel *d2Model)
	: UnrealSensorImplementation(port, d2Model, robots::enums::sensorType::sonar)
{
}

void UnrealSonarSensorImplementation::read()
{
	emit response(mD2Model->readSonarSensor(mPort));
}

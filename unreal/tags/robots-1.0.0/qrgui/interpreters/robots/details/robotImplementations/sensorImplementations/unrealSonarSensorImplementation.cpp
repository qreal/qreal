#include "unrealSonarSensorImplementation.h"
#include <QtCore/QDebug>
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealSonarSensorImplementation::UnrealSonarSensorImplementation(inputPort::InputPortEnum const &port, d2Model::D2RobotModel *d2Model)
	: UnrealSensorImplementation(port, d2Model)
{
}

void UnrealSonarSensorImplementation::read()
{
	emit response(13);
}

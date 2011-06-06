#include "unrealColorSensorImplementation.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

UnrealColorSensorImplementation::UnrealColorSensorImplementation(inputPort::InputPortEnum const &port, d2Model::D2RobotModel *d2Model)
	: UnrealSensorImplementation(port, d2Model)
{
}

void UnrealColorSensorImplementation::read()
{
	emit mD2Model->readColorSensor(mPort);
}

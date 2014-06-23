#include "movementSensor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

MovementSensor::MovementSensor(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikMovementSensor(info, port)
{
}

void MovementSensor::read()
{
	emit newData(0);
}

#include "trikMovementSensor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikMovementSensor::TrikMovementSensor(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::ScalarSensor(info, port)
{
}

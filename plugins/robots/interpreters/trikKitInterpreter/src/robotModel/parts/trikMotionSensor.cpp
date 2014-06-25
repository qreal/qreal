#include "trikMotionSensor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikMotionSensor::TrikMotionSensor(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::ScalarSensor(info, port)
{
}

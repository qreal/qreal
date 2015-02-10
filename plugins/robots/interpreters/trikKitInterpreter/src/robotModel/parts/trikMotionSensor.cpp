#include "trikMotionSensor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikMotionSensor::TrikMotionSensor(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::ScalarSensor(info, port)
{
}

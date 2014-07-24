#include "trikObjectSensor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikObjectSensor::TrikObjectSensor(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::VectorSensor(info, port)
{
}

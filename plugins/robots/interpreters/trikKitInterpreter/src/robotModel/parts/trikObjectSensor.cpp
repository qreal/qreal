#include "trikObjectSensor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikObjectSensor::TrikObjectSensor(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::VectorSensor(info, port)
{
}

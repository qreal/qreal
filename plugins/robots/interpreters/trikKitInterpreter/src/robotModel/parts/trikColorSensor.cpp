#include "trikColorSensor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikColorSensor::TrikColorSensor(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::VectorSensor(info, port)
{
}

#include "trikColorSensor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikColorSensor::TrikColorSensor(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::VectorSensor(info, port)
{
}

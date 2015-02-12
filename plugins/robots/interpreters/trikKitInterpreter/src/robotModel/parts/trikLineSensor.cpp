#include "trikLineSensor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikLineSensor::TrikLineSensor(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::VectorSensor(info, port)
{
}

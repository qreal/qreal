#include "trikInfraredSensor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikInfraredSensor::TrikInfraredSensor(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::RangeSensor(info, port)
{
}

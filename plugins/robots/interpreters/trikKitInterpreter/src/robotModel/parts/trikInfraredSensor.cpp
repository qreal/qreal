#include "trikInfraredSensor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikInfraredSensor::TrikInfraredSensor(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::RangeSensor(info, port)
{
}

#include "trikKit/robotModel/parts/trikInfraredSensor.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikInfraredSensor::TrikInfraredSensor(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::RangeSensor(info, port)
{
}

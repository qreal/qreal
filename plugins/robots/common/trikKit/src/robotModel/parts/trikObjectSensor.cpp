#include "trikKit/robotModel/parts/trikObjectSensor.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikObjectSensor::TrikObjectSensor(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::VectorSensor(info, port)
{
}

#include "trikKit/robotModel/parts/trikColorSensor.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikColorSensor::TrikColorSensor(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::VectorSensor(info, port)
{
}

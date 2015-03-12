#include "trikKit/robotModel/parts/trikLineSensor.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikLineSensor::TrikLineSensor(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::VectorSensor(info, port)
{
}

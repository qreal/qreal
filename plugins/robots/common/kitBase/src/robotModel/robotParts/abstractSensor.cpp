#include "kitBase/robotModel/robotParts/abstractSensor.h"

using namespace kitBase::robotModel::robotParts;

AbstractSensor::AbstractSensor(const DeviceInfo &info, const PortInfo &port)
	: Device(info, port)
{
}

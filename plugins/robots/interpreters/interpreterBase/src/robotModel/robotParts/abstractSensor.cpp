#include "interpreterBase/robotModel/robotParts/abstractSensor.h"

using namespace interpreterBase::robotModel::robotParts;

AbstractSensor::AbstractSensor(const DeviceInfo &info, const PortInfo &port)
	: Device(info, port)
{
}

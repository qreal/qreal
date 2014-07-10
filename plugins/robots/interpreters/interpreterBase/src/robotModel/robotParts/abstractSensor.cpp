#include "interpreterBase/robotModel/robotParts/abstractSensor.h"

using namespace interpreterBase::robotModel::robotParts;

AbstractSensor::AbstractSensor(DeviceInfo const &info, PortInfo const &port)
	: Device(info, port)
{
}

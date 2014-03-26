#include "interpreterBase/robotModel/robotParts/abstractSensor.h"

using namespace interpreterBase::robotModel::robotParts;

interpreterBase::robotModel::DeviceInfo::Direction AbstractSensor::direction()
{
	return DeviceInfo::input;
}

AbstractSensor::AbstractSensor(DeviceInfo const &info, PortInfo const &port)
	: Device(info, port)
{
}

AbstractSensor::~AbstractSensor()
{
}

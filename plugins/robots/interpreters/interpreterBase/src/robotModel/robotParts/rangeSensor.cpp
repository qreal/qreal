#include "interpreterBase/robotModel/robotParts/rangeSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

QString RangeSensor::friendlyName()
{
	return tr("Range sensor");
}

DeviceInfo::Direction RangeSensor::direction()
{
	return DeviceInfo::input;
}

RangeSensor::RangeSensor(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}

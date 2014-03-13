#include "interpreterBase/robotModel/robotParts/gyroscopeSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

QString GyroscopeSensor::friendlyName()
{
	return tr("Gyroscope");
}

DeviceInfo::Direction GyroscopeSensor::direction()
{
	return DeviceInfo::input;
}

GyroscopeSensor::GyroscopeSensor(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}

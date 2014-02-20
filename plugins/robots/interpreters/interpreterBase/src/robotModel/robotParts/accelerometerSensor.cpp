#include "interpreterBase/robotModel/robotParts/accelerometerSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

QString AccelerometerSensor::friendlyName()
{
	return tr("Accelerometer sensor");
}

DeviceInfo::Direction AccelerometerSensor::direction()
{
	return DeviceInfo::input;
}

AccelerometerSensor::AccelerometerSensor(DeviceInfo const &info, PortInfo const &port)
	: VectorSensor(info, port)
{
}

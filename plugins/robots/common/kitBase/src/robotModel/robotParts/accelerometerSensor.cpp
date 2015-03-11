#include "kitBase/robotModel/robotParts/accelerometerSensor.h"

using namespace kitBase::robotModel;
using namespace robotParts;

AccelerometerSensor::AccelerometerSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}

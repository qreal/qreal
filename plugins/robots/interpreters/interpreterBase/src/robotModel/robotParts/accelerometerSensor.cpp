#include "interpreterBase/robotModel/robotParts/accelerometerSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

AccelerometerSensor::AccelerometerSensor(const DeviceInfo &info, const PortInfo &port)
	: ScalarSensor(info, port)
{
}

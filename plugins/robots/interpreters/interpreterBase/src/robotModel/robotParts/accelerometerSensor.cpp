#include "interpreterBase/robotModel/robotParts/accelerometerSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

AccelerometerSensor::AccelerometerSensor(DeviceInfo const &info, PortInfo const &port)
	: ScalarSensor(info, port)
{
}

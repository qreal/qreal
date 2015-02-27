#include "kitBase/robotModel/robotParts/touchSensor.h"

using namespace kitBase::robotModel;
using namespace robotParts;

TouchSensor::TouchSensor(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port)
	: ScalarSensor(info, port)
{
}

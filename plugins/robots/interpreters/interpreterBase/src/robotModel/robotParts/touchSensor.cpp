#include "interpreterBase/robotModel/robotParts/touchSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

TouchSensor::TouchSensor(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port)
	: ScalarSensor(info, port)
{
}

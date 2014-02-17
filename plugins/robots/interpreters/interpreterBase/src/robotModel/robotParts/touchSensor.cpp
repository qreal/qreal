#include "interpreterBase/robotModel/robotParts/touchSensor.h"

using namespace interpreterBase::robotModel::robotParts;

TouchSensor::TouchSensor(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port)
	: ScalarSensor(info, port)
{
}

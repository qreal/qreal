#include "interpreterBase/robotModel/robotParts/touchSensor.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

QString TouchSensor::friendlyName()
{
	return tr("Touch sensor");
}

DeviceInfo::Direction TouchSensor::direction()
{
	return DeviceInfo::input;
}

TouchSensor::TouchSensor(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port)
	: ScalarSensor(info, port)
{
}

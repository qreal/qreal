#include "interpreterBase/robotModel/robotParts/scalarSensor.h"

using namespace interpreterBase::robotModel::robotParts;

QString ScalarSensor::friendlyName()
{
	return QString();
}

ScalarSensor::ScalarSensor(DeviceInfo const &info, PortInfo const &port)
	: AbstractSensor(info, port)
{
}

ScalarSensor::~ScalarSensor()
{
}

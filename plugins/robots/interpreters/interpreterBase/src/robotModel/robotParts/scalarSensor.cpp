#include "interpreterBase/robotModel/robotParts/scalarSensor.h"

using namespace interpreterBase::robotModel::robotParts;

ScalarSensor::ScalarSensor(const DeviceInfo &info, const PortInfo &port)
	: AbstractSensor(info, port)
	, mLastValue(0)
{
	connect(this, &ScalarSensor::newData, [this](int reading) { mLastValue = reading; });
}

int ScalarSensor::lastData() const
{
	return mLastValue;
}

#include "kitBase/robotModel/robotParts/scalarSensor.h"

using namespace kitBase::robotModel::robotParts;

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

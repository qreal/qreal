#include "kitBase/robotModel/robotParts/vectorSensor.h"

using namespace kitBase::robotModel::robotParts;

VectorSensor::VectorSensor(const DeviceInfo &info, const PortInfo &port)
	: AbstractSensor(info, port)
{
	connect(this, &VectorSensor::newData, [this](const QVector<int> &reading) { mLastValue = reading; });
}

QVector<int> VectorSensor::lastData() const
{
	return mLastValue;
}

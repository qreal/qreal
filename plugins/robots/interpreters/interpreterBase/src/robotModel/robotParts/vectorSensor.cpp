#include "interpreterBase/robotModel/robotParts/vectorSensor.h"

using namespace interpreterBase::robotModel::robotParts;

VectorSensor::VectorSensor(const DeviceInfo &info, const PortInfo &port)
	: AbstractSensor(info, port)
{
	connect(this, &VectorSensor::newData, [this](const QVector<int> &reading) { mLastValue = reading; });
}

QVector<int> VectorSensor::lastData() const
{
	return mLastValue;
}

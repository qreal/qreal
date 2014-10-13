#include "accelerometer.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Accelerometer::Accelerometer(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::AccelerometerSensor(info, port)
{
}

void Accelerometer::read()
{
	emit newData(0);
}

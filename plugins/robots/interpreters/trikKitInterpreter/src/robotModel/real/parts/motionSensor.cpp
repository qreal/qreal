#include "motionSensor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

MotionSensor::MotionSensor(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikMotionSensor(info, port)
{
}

void MotionSensor::read()
{
	emit newData(0);
}

#include "trikKit/robotModel/parts/trikMotionSensor.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikMotionSensor::TrikMotionSensor(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::ScalarSensor(info, port)
{
}

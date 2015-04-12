#include "trikKit/robotModel/parts/trikSonarSensor.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikSonarSensor::TrikSonarSensor(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::RangeSensor(info, port)
{
}

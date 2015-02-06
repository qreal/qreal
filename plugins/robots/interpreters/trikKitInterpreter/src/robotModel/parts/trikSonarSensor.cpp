#include "trikSonarSensor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikSonarSensor::TrikSonarSensor(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::RangeSensor(info, port)
{
}

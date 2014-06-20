#include "trikSonarSensor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikSonarSensor::TrikSonarSensor(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::RangeSensor(info, port)
{
}

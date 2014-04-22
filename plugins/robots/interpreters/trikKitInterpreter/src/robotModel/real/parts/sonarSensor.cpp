#include "sonarSensor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

SonarSensor::SonarSensor(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikSonarSensor(info, port)
{
}

void SonarSensor::read()
{
	emit newData(0);
}

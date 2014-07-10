#include "infraredSensor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

InfraredSensor::InfraredSensor(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikInfraredSensor(info, port)
{
}

void InfraredSensor::read()
{
	emit newData(0);
}

#include "gyroscope.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Gyroscope::Gyroscope(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::GyroscopeSensor(info, port)
{
}

void Gyroscope::read()
{
	emit newData(0);
}

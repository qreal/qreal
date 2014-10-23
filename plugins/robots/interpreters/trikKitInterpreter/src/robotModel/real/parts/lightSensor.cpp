#include "lightSensor.h"
#include <utils/tracer.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

int const maxLightValue = 1023;

LightSensor::LightSensor(DeviceInfo const &info, PortInfo const &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: interpreterBase::robotModel::robotParts::LightSensor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void LightSensor::read()
{
	emit newData(0);
}

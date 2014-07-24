#include "infraredSensor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

InfraredSensor::InfraredSensor(DeviceInfo const &info, PortInfo const &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikInfraredSensor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void InfraredSensor::read()
{
	emit newData(0);
}

#include "sonarSensor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

SonarSensor::SonarSensor(DeviceInfo const &info, PortInfo const &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikSonarSensor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
}

void SonarSensor::read()
{
	emit newData(0);
}

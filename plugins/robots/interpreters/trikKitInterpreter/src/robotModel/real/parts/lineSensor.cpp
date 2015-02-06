#include "lineSensor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

LineSensor::LineSensor(const DeviceInfo &info, const PortInfo &port, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikLineSensor(info, port), mRobotCommunicator(tcpRobotCommunicator)
{
}

void LineSensor::init()
{
}

void LineSensor::detectLine()
{
}

void LineSensor::read()
{
	emit newData({0, 0, 0});
}

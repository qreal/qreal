#include "lineSensor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

LineSensor::LineSensor(DeviceInfo const &info, PortInfo const &port, utils::TcpRobotCommunicator &tcpRobotCommunicator)
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

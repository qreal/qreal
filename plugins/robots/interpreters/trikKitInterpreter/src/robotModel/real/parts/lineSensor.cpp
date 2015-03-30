#include "lineSensor.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

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
	emit newData({});
}

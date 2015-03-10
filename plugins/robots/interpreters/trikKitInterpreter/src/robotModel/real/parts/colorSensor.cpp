#include "colorSensor.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

ColorSensor::ColorSensor(const DeviceInfo &info, const PortInfo &port, utils::TcpRobotCommunicator &robotCommunicator)
	: robotModel::parts::TrikColorSensor(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void ColorSensor::init()
{
}

void ColorSensor::read()
{
	emit newData({0, 0, 0});
}

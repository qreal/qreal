#include "twoDLineSensor.h"

using namespace trik::robotModel::twoD::parts;
using namespace kitBase::robotModel;

LineSensor::LineSensor(const DeviceInfo &info, const PortInfo &port)
	: robotModel::parts::TrikLineSensor(info, port)
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

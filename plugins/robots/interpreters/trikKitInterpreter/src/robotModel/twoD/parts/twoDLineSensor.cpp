#include "twoDLineSensor.h"

using namespace trikKitInterpreter::robotModel::twoD::parts;
using namespace interpreterBase::robotModel;

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

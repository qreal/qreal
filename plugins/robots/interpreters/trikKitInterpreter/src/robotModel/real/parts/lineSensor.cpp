#include "lineSensor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

LineSensor::LineSensor(DeviceInfo const &info, PortInfo const &port)
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

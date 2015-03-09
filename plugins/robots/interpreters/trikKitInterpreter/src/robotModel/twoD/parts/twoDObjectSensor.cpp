#include "twoDObjectSensor.h"

using namespace trikKitInterpreter::robotModel::twoD::parts;
using namespace interpreterBase::robotModel;

ObjectSensor::ObjectSensor(const DeviceInfo &info, const PortInfo &port)
	: robotModel::parts::TrikObjectSensor(info, port)
{
}

void ObjectSensor::init()
{
}

void ObjectSensor::detect()
{
}

void ObjectSensor::read()
{
	emit newData({0, 0, 0});
}

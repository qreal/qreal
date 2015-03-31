#include "twoDObjectSensor.h"

using namespace trik::robotModel::twoD::parts;
using namespace kitBase::robotModel;

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
	emit newData({});
}

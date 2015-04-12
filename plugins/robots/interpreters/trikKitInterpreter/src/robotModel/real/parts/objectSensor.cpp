#include "objectSensor.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

ObjectSensor::ObjectSensor(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &robotCommunicator)
	: robotModel::parts::TrikObjectSensor(info, port)
	, mRobotCommunicator(robotCommunicator)
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

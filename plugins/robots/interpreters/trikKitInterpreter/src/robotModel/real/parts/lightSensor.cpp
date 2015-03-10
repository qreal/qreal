#include "lightSensor.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

const int maxLightValue = 1023;

LightSensor::LightSensor(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: kitBase::robotModel::robotParts::LightSensor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
			, this, &LightSensor::onIncomingData);
}

void LightSensor::read()
{
	mRobotCommunicator.requestData(port().name());
}

void LightSensor::onIncomingData(const QString &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}

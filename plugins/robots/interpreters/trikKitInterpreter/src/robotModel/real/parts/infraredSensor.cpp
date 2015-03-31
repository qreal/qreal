#include "infraredSensor.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

InfraredSensor::InfraredSensor(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikInfraredSensor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
			, this, &InfraredSensor::onIncomingData);
}

void InfraredSensor::read()
{
	mRobotCommunicator.requestData(port().name());
}

void InfraredSensor::onIncomingData(const QString &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}


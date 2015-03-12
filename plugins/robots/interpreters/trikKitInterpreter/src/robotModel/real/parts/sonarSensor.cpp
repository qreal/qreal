#include "sonarSensor.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

SonarSensor::SonarSensor(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikSonarSensor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
			, this, &SonarSensor::onIncomingData);
}

void SonarSensor::read()
{
	mRobotCommunicator.requestData(port().name());
}

void SonarSensor::onIncomingData(const QString &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}

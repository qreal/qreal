#include "sonarSensor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

SonarSensor::SonarSensor(DeviceInfo const &info, PortInfo const &port
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

void SonarSensor::onIncomingData(QString const &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}

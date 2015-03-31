#include "gyroscope.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

Gyroscope::Gyroscope(const DeviceInfo &info, const PortInfo &port, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: kitBase::robotModel::robotParts::GyroscopeSensor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
			, this, &Gyroscope::onIncomingData);
}

void Gyroscope::read()
{
	mRobotCommunicator.requestData(port().name());
}

void Gyroscope::onIncomingData(const QString &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}

#include "button.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

Button::Button(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotParts::Button(info, port), mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
			, this, &Button::onIncomingData);
}

void Button::read()
{
	mRobotCommunicator.requestData("button:" + port().name());
}

void Button::onIncomingData(const QString &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}

#include "gamepadWheel.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

GamepadWheel::GamepadWheel(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikGamepadWheel(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
			, this, &GamepadWheel::onIncomingData);
}

void GamepadWheel::read()
{
	mRobotCommunicator.requestData(port().name());
}

void GamepadWheel::onIncomingData(const QString &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}

#include "gamepadButton.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

GamepadButton::GamepadButton(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikGamepadButton(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
			, this, &GamepadButton::onIncomingData);
}

void GamepadButton::read()
{
	mRobotCommunicator.requestData(port().name());
}

void GamepadButton::onIncomingData(const QString &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}

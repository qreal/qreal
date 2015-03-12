#include "gamepadConnectionIndicator.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

GamepadConnectionIndicator::GamepadConnectionIndicator(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikGamepadConnectionIndicator(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
			, this, &GamepadConnectionIndicator::onIncomingData);
}

void GamepadConnectionIndicator::read()
{
	mRobotCommunicator.requestData(port().name());
}

void GamepadConnectionIndicator::onIncomingData(const QString &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}

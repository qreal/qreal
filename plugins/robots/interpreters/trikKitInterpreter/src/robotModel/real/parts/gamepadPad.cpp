#include "gamepadPad.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

GamepadPad::GamepadPad(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikGamepadPad(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newVectorSensorData
			, this, &GamepadPad::onIncomingData);
}

void GamepadPad::read()
{
	mRobotCommunicator.requestData(port().name());
}

void GamepadPad::onIncomingData(const QString &portName, const QVector<int> &value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}

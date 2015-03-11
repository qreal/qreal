#include "gamepadPadPressSensor.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

GamepadPadPressSensor::GamepadPadPressSensor(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikGamepadPadPressSensor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
			, this, &GamepadPadPressSensor::onIncomingData);
}

void GamepadPadPressSensor::read()
{
	mRobotCommunicator.requestData(port().name());
}

void GamepadPadPressSensor::onIncomingData(const QString &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}

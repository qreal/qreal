#include "gamepadPadPressSensor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

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

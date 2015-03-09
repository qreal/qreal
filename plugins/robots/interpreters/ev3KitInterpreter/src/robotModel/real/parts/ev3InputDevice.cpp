#include "ev3InputDevice.h"

#include <utils/tracer.h>

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;
using namespace utils;

Ev3InputDevice::Ev3InputDevice(
		utils::robotCommunication::RobotCommunicator &robotCommunicator
		, const interpreterBase::robotModel::PortInfo &port
		, SensorTypeEnum lowLevelSensorType
		, INPUT_DEVICE_SUBCODE sensorMode)
	: mRobotCommunicator(robotCommunicator)
	, mLowLevelPort(port.name().at(0).toLatin1() - '1')
	, mLowLevelSensorType(lowLevelSensorType)
	, mSensorMode(sensorMode)
{
}

void Ev3InputDevice::send(QByteArray command, const unsigned responseSize, QByteArray reading)
{
	mRobotCommunicator.send(command, responseSize, reading);
}

char Ev3InputDevice::lowLevelPort() const
{
	return mLowLevelPort;
}

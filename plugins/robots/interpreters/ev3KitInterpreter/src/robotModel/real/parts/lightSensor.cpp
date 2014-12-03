#include "lightSensor.h"

unsigned const lightSensorResponseSize = 9;

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

LightSensor::LightSensor(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: robotParts::LightSensor(info, port)
	, mImplementation(robotCommunicator, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void LightSensor::read()
{
	QByteArray command = mImplementation.readyPercentCommand(mImplementation.lowLevelPort(), 0);
	QByteArray outputBuf;
	mRobotCommunicator.send(command, lightSensorResponseSize, outputBuf);
	emit newData(static_cast<int>(outputBuf.data()[5]));
}

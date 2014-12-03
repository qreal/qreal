#include "touchSensor.h"
#include <QDebug>

unsigned const touchSensorResponseSize = 9;
unsigned const pressed = 63;

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

TouchSensor::TouchSensor(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: robotParts::TouchSensor(info, port)
	, mImplementation(robotCommunicator, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void TouchSensor::read()
{
	QByteArray command = mImplementation.readySiCommand(mImplementation.lowLevelPort(), 0);
	QByteArray outputBuf;
	mRobotCommunicator.send(command, touchSensorResponseSize, outputBuf);
	if (outputBuf.data()[8] == pressed) {
		emit newData(1);
	} else {
		emit newData(0);
	}
}


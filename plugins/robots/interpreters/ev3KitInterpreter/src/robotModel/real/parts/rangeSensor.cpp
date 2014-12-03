#include "rangeSensor.h"
#include <QDebug>

unsigned const rangeSensorResponseSize = 9;

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

RangeSensor::RangeSensor(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: robotParts::RangeSensor(info, port)
	, mImplementation(robotCommunicator, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void RangeSensor::read()
{
	QByteArray command = mImplementation.readyRawCommand(mImplementation.lowLevelPort(), 0);
	QByteArray outputBuf;
	mRobotCommunicator.send(command, rangeSensorResponseSize, outputBuf);
	emit newData(static_cast<int>(outputBuf.data()[5]));
}


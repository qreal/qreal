#include "touchSensor.h"

using namespace qReal::interpreters::robots::details::robotParts;

TouchSensor::TouchSensor(RobotCommunicationInterface *robotCommunicationInterface)
	: RobotPart(robotCommunicationInterface)
{
}

void TouchSensor::read()
{
	QByteArray command(5, 0);
	command[0] = 0x03;  //command length
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = 0x07;
	command[4] = mPort;
	mRobotCommunicationInterface->send(this, command);
}

void TouchSensor::processResponse(QByteArray const &reading)
{
	qDebug() << "Touch sensor response:" << reading;
}

void TouchSensor::configure(int port)
{
	QByteArray command(7, 0);
	command[0] = 0x05;  //command length
	command[1] = 0x00;
	command[2] = 0x80;  // reply not needed for now
	command[3] = 0x05;
	command[4] = 0x00;  // sensor port. always 0 for now.
	command[5] = 0x01;  // sensor type: touch
	command[6] = 0x00;  // sensor mode: raw
	mRobotCommunicationInterface->send(this, command);
}

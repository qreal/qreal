#include "brick.h"

using namespace qReal::interpreters::robots::details::robotParts;

Brick::Brick(RobotCommunicationInterface *robotCommunicationInterface)
	: RobotPart(robotCommunicationInterface)
{
}

void Brick::playTone(unsigned freq, unsigned time)
{
	QByteArray command(8, 0);
	command[0] = 0x06;  //command length
	command[1] = 0x00;
	command[2] = 0x80;
	command[3] = 0x03;
	command[4] = freq;
	command[5] = freq >> 8;
	command[6] = time;
	command[7] = time >> 8;
	mRobotCommunicationInterface->send(this, command, 5);
}

void Brick::beep(unsigned time)
{
	playTone(1000, time);
}

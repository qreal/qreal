#include "realBrickImplementation.h"
#include "../../robotCommandConstants.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::brickImplementations;

RealBrickImplementation::RealBrickImplementation(RobotCommunicationInterface * const robotCommunicationInterface)
	: mRobotCommunicationInterface(robotCommunicationInterface)
{
}

void RealBrickImplementation::playTone(unsigned freq, unsigned time)
{
	QByteArray command(8, 0);
	command[0] = 0x06;  //command length
	command[1] = 0x00;
	command[2] = telegramType::directCommandNoResponse;
	command[3] = commandCode::PLAYTONE;
	command[4] = freq;
	command[5] = freq >> 8;
	command[6] = time;
	command[7] = time >> 8;
	mRobotCommunicationInterface->send(this, command, 5);
}

void RealBrickImplementation::beep(unsigned time)
{
	playTone(1000, time);
}

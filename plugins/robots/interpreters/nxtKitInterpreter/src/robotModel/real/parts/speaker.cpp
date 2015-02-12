#include "speaker.h"
#include "commandConstants.h"

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Speaker::Speaker(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: robotModel::parts::NxtSpeaker(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void Speaker::playTone(unsigned freq, unsigned time)
{
	QByteArray command(8, 0);
	command[0] = 0x06;  //command length
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandNoResponse;
	command[3] = enums::commandCode::PLAYTONE;
	command[4] = freq;
	command[5] = freq >> 8;
	command[6] = time;
	command[7] = time >> 8;
	mRobotCommunicator.send(this, command, 5);
}

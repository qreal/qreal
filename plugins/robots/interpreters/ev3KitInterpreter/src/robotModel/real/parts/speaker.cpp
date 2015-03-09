#include "speaker.h"
#include "commandConstants.h"

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Speaker::Speaker(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
		: robotModel::parts::Ev3Speaker(info, port)
		, mRobotCommunicator(robotCommunicator)
{
}

void Speaker::playTone(int volume, int frequency, int duration)
{
	QByteArray command(17, 0);
	command[0] = 15;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = 0x00;
	command[4] = DIRECT_COMMAND_NO_REPLY;
	const int globalVariablesCount = 0;
	const int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opSOUND;
	command[8] = LC0(TONE);
	//LC1(volume)
	command[9] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[10] = (volume & 0xFF);
	//LC2(frequency)
	command[11] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[12] = (frequency & 0xFF);
	command[13] = ((frequency >> 8) & 0xFF);
	//LC2(duration)
	command[14] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[15] = (duration & 0xFF);
	command[16] = ((duration >> 8) & 0xFF);
	mRobotCommunicator.send(this, command, 3);
}

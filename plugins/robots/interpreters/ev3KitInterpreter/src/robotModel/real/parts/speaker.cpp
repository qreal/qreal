#include "speaker.h"

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Speaker::Speaker(DeviceInfo const &info, PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
		: robotModel::parts::Ev3Speaker(info, port)
		, mRobotCommunicator(robotCommunicator)
{
}

void Speaker::playTone(int volume, int frequency, int duration)
{
	QByteArray command = Ev3DirectCommand::formCommand(17, 0, 0, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::SOUND_TONE, command, index);
	Ev3DirectCommand::addByteParameter(volume, command, index);
	Ev3DirectCommand::addShortParameter(frequency, command, index);
	Ev3DirectCommand::addShortParameter(duration, command, index);
	mRobotCommunicator.send(this, command, 3);
}

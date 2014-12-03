#include "ev3DirectCommand.h"

using namespace ev3KitInterpreter::robotModel::real;

Ev3DirectCommand::Ev3DirectCommand()
{
}

QByteArray Ev3DirectCommand::formCommand(int size, int messageCounter, ushort globalSize, int localSize, enums::commandType::CommandTypeEnum type)
{
	QByteArray command(size, 0);
	command[0] = (size - 2) & 0xFF;
	command[1] = (size >> 8) & 0xFF ;
	command[2] = messageCounter & 0xFF;
	command[3] = (messageCounter >> 8) & 0xFF;
	command[4] = type;
	command[5] = globalSize & 0xFF;
	command[6] = ((localSize << 2) | (globalSize >> 8));

	return command;
}

void Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum opcode, QByteArray &command, int &index)
{
	if(opcode > enums::opcode::OpcodeEnum::TST) {
		command[index++] = (opcode >> 8) & 0xFF;
	}
	command[index++] = opcode & 0xFF;
}

void Ev3DirectCommand::addByteParameter(qint8 parameter, QByteArray &command, int &index)
{
	command[index++] = enums::argumentSize::ArgumentSizeEnum::BYTE;
	command[index++] = parameter & 0xFF;
}

void Ev3DirectCommand::addShortParameter(qint16 parameter, QByteArray &command, int &index)
{
	command[index++] = enums::argumentSize::ArgumentSizeEnum::SHORT;
	command[index++] = parameter & 0xFF;
	command[index++] = (parameter >> 8) & 0xFF;
}

void Ev3DirectCommand::addStringParameter(QString const &parameter, QByteArray &command, int &index)
{
	QByteArray parameterBytes = parameter.toLocal8Bit();
	command[index++] = enums::argumentSize::ArgumentSizeEnum::STRING;
	for (int i = 0; i < parameter.length(); i++) {
		command[index++] = parameterBytes[i];
	}
	command[index++] = 0x00;
}

void Ev3DirectCommand::addGlobalIndex(qint8 globalIndex, QByteArray &command, int &index)
{
	command[index++] = 0xE1; // 0xE1 = global index, long format, 1 byte
	command[index++] = globalIndex & 0xFF;
}

#include "button.h"

unsigned const buttonResponseSize = 6;

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Button::Button(DeviceInfo const &info, PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: robotParts::Button(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void Button::read()
{
	int const size = 13;
	QByteArray command(size, 0);
	command[0] = size - 2;
	command[1] = size >> 8;
	command[2] = 4;
	command[3] = 0;
	command[4] = DIRECT_COMMAND_REPLY;
	int const globalVariablesCount = 1;
	int const localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opUI_BUTTON;
	command[8] = PRESSED;
	command[9] = (PRIMPAR_LONG | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[10] = parsePort(port().name()) & 0xFF;
	command[11] = 225;
	command[12] = 0;
	QByteArray outputBuf;
	mRobotCommunicator.send(command, buttonResponseSize, outputBuf);
	if (outputBuf.length() == 6 && outputBuf.data()[5] == 0x01) {
		emit newData(1);
	} else {
		emit newData(0);
	}
}

char Button::parsePort(QString portName)
{
	if (portName == "UpButtonPort") {
		return enums::brickButton::BrickButtonEnum::UP;
	} else if (portName == "EnterButtonPort") {
		return enums::brickButton::BrickButtonEnum::ENTER;
	} else if (portName == "DownButtonPort") {
		return enums::brickButton::BrickButtonEnum::DOWN;
	} else if (portName == "RightButtonPort") {
		return enums::brickButton::BrickButtonEnum::RIGHT;
	} else if (portName == "LeftButtonPort") {
		return enums::brickButton::BrickButtonEnum::LEFT;
	} else if (portName == "BackButtonPort") {
		return enums::brickButton::BrickButtonEnum::BACK;
	}

	return enums::brickButton::BrickButtonEnum::ANY;
}

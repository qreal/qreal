#include "button.h"

unsigned const buttonResponseSize = 6;
unsigned const buttonPressed = 0x01;

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
	QByteArray command = Ev3DirectCommand::formCommand(13, 4, 1, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_BUTTON_PRESSED, command, index);
	Ev3DirectCommand::addByteParameter(parsePort(port().name()), command, index);
	Ev3DirectCommand::addGlobalIndex(0, command, index);
	QByteArray outputBuf;
	mRobotCommunicator.send(command, buttonResponseSize, outputBuf);
	if (outputBuf.length() == buttonResponseSize && outputBuf.data()[5] == buttonPressed) {
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

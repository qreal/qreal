#include "display.h"

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase;
using namespace robotModel;
using namespace utils;
using namespace robotCommunication;

Display::Display(DeviceInfo const &info, PortInfo const &port, RobotCommunicator &robotCommunicator)
	: robotModel::parts::Ev3Display(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void Display::drawPixel(int x, int y)
{
	QByteArray command = Ev3DirectCommand::formCommand(19, 0, 0, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::UI_DRAW_PIXEL, command, index);
	Ev3DirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	Ev3DirectCommand::addShortParameter(x, command, index);
	Ev3DirectCommand::addShortParameter(y, command, index);
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);
	mRobotCommunicator.send(this, command, 3);
}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	QByteArray command = Ev3DirectCommand::formCommand(25, 0, 0, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_LINE, command, index);
	Ev3DirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	Ev3DirectCommand::addShortParameter(x1, command, index);
	Ev3DirectCommand::addShortParameter(y1, command, index);
	Ev3DirectCommand::addShortParameter(x2, command, index);
	Ev3DirectCommand::addShortParameter(y2, command, index);
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);
	mRobotCommunicator.send(this, command, 3);
}

void Display::drawRect(int x, int y, int width, int height, bool filled)
{
	QByteArray command = Ev3DirectCommand::formCommand(25, 0, 0, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	if (filled) {
		Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_FILL_RECT, command, index);
	} else {
		Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_RECT, command, index);
	}
	Ev3DirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	Ev3DirectCommand::addShortParameter(x, command, index);
	Ev3DirectCommand::addShortParameter(y, command, index);
	Ev3DirectCommand::addShortParameter(width, command, index);
	Ev3DirectCommand::addShortParameter(height, command, index);
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);
	mRobotCommunicator.send(this, command, 3);
}

void Display::drawRect(int x, int y, int width, int height)
{
	Q_UNUSED(x);
	Q_UNUSED(y);
	Q_UNUSED(width);
	Q_UNUSED(height);

}

void Display::drawCircle(int x, int y, int radius, bool filled)
{
	QByteArray command = Ev3DirectCommand::formCommand(22, 0, 0, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	if (filled) {
		Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_FILL_CIRCLE, command, index);
	} else {
		Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_CIRCLE, command, index);
	}
	Ev3DirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	Ev3DirectCommand::addShortParameter(x, command, index);
	Ev3DirectCommand::addShortParameter(y, command, index);
	Ev3DirectCommand::addShortParameter(radius, command, index);
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);
	mRobotCommunicator.send(this, command, 3);
}


void Display::printText(int x, int y, QString const &text)
{
	QByteArray command = Ev3DirectCommand::formCommand(21 + text.length(), 0, 0, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_TEXT, command, index);
	Ev3DirectCommand::addByteParameter(enums::color::ColorEnum::FOREGROUND, command, index);
	Ev3DirectCommand::addShortParameter(x, command, index);
	Ev3DirectCommand::addShortParameter(y, command, index);
	Ev3DirectCommand::addStringParameter(text, command, index);
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);
	mRobotCommunicator.send(this, command, 3);
}

void Display::clearScreen()
{
	QByteArray command = Ev3DirectCommand::formCommand(11, 0, 0, 0, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_NO_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_CLEAN, command, index);
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::UI_DRAW_UPDATE, command, index);
	mRobotCommunicator.send(this, command, 3);
}

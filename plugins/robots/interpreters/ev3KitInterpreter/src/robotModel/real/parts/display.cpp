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
	QByteArray command(18, 0);
	command[0] = 16;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = 0x00;
	command[4] = DIRECT_COMMAND_NO_REPLY;
	int globalVariablesCount = 0;
	int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opUI_DRAW;
	command[8] = LC0(PIXEL);
	command[9] = LC0(vmFG_COLOR);
	//LC2(x)
	command[10] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[11] = (x & 0xFF);
	command[12] = ((x >> 8) & 0xFF);
	//LC2(y)
	command[13] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[14] = (y & 0xFF);
	command[15] = ((y >> 8) & 0xFF);
	command[16] = opUI_DRAW;
	command[17] = LC0(UPDATE);
	mRobotCommunicator.send(this, command, 3);
}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	QByteArray command(24, 0);
	command[0] = 22;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = 0x00;
	command[4] = DIRECT_COMMAND_NO_REPLY;
	int globalVariablesCount = 0;
	int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opUI_DRAW;
	command[8] = LC0(LINE);
	command[9] = LC0(vmFG_COLOR);
	//LC2(x0)
	command[10] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[11] = (x1 & 0xFF);
	command[12] = ((x2 >> 8) & 0xFF);
	//LC2(y0)
	command[13] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[14] = (y1 & 0xFF);
	command[15] = ((y1 >> 8) & 0xFF);
	//LC2(x1)
	command[16] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[17] = (x2 & 0xFF);
	command[18] = ((x2 >> 8) & 0xFF);
	//LC2(y1)
	command[19] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[20] = (y2 & 0xFF);
	command[21] = ((y2 >> 8) & 0xFF);
	command[22] = opUI_DRAW;
	command[23] = LC0(UPDATE);
	mRobotCommunicator.send(this, command, 3);
}

void Display::drawRect(int x, int y, int width, int height)
{
	QByteArray command(24, 0);
	command[0] = 22;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = 0x00;
	command[4] = DIRECT_COMMAND_NO_REPLY;
	int globalVariablesCount = 0;
	int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opUI_DRAW;
	command[8] = LC0(RECT);
	command[9] = LC0(vmFG_COLOR);
	//LC2(x0)
	command[10] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[11] = (x & 0xFF);
	command[12] = ((x >> 8) & 0xFF);
	//LC2(y0)
	command[13] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[14] = (y & 0xFF);
	command[15] = ((y >> 8) & 0xFF);
	//LC2(x1)
	command[16] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[17] = (width & 0xFF);
	command[18] = ((width >> 8) & 0xFF);
	//LC2(y1)
	command[19] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[20] = (height & 0xFF);
	command[21] = ((height >> 8) & 0xFF);
	command[22] = opUI_DRAW;
	command[23] = LC0(UPDATE);
	mRobotCommunicator.send(this, command, 3);
}

void Display::drawCircle(int x, int y, int radius)
{
	QByteArray command(21, 0);
	command[0] = 19;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = 0x00;
	command[4] = DIRECT_COMMAND_NO_REPLY;
	int globalVariablesCount = 0;
	int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opUI_DRAW;
	command[8] = LC0(CIRCLE);
	command[9] = LC0(vmFG_COLOR);
	//LC2(x)
	command[10] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[11] = (x & 0xFF);
	command[12] = ((x >> 8) & 0xFF);
	//LC2(y)
	command[13] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[14] = (y & 0xFF);
	command[15] = ((y >> 8) & 0xFF);
	//LC2(radius)
	command[16] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[17] = (radius & 0xFF);
	command[18] = ((radius >> 8) & 0xFF);
	command[19] = opUI_DRAW;
	command[20] = LC0(UPDATE);
	mRobotCommunicator.send(this, command, 3);
}

void Display::printText(int x, int y, QString const &text)
{
	int size = 20 + text.length();
	QByteArray textBytes = text.toLocal8Bit();
	QByteArray command(size, 0);
	command[0] = size - 2;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = 0x00;
	command[4] = DIRECT_COMMAND_NO_REPLY;
	int globalVariablesCount = 0;
	int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opUI_DRAW;
	command[8] = TEXT;
	command[9] = LC0(vmFG_COLOR);
	//LC2(x)
	command[10] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[11] = (x & 0xFF);
	command[12] = ((x >> 8) & 0xFF);
	//LC2(y)
	command[13] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_2_BYTES);
	command[14] = (y & 0xFF);
	command[15] = ((y >> 8) & 0xFF);
	command[16] = LCS;
	int currentSymbol = 17;
	for (int i = 0; i < textBytes.length(); i++)
	{
		command[currentSymbol] = textBytes[i];
		currentSymbol++;
	}
	command[size - 3] = 0;
	command[size - 2] = opUI_DRAW;
	command[size - 1] = LC0(UPDATE);
	mRobotCommunicator.send(this, command, 3);
}

void Display::clearScreen()
{
	QByteArray command(11, 0);
	command[0] = 9;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = 0x00;
	command[4] = DIRECT_COMMAND_NO_REPLY;
	int globalVariablesCount = 0;
	int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opUI_DRAW;
	command[8] = LC0(CLEAN);
	command[9] = opUI_DRAW;
	command[10] = LC0(UPDATE);
	mRobotCommunicator.send(this, command, 3);
}

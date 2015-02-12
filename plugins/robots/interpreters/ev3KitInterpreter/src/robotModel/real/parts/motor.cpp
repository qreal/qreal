#include "motor.h"

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase;
using namespace robotModel;
using namespace utils;
using namespace robotCommunication;

Motor::Motor(const DeviceInfo &info, const PortInfo &port, RobotCommunicator &robotCommunicator)
	: Ev3Motor(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void Motor::on(int speed)
{
	QByteArray command(15, 0);
	command[0] = 13;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = 0x00;
	command[4] = DIRECT_COMMAND_NO_REPLY;
	const int globalVariablesCount = 0;
	const int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opOUTPUT_POWER;
	command[8] = LC0(0); //layer(EV3)
	command[9] = parsePort(port().name().at(0).toLatin1());
	command[10] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[11] = speed & 0xFF;
	command[12] = opOUTPUT_START;
	command[13] = LC0(0); // layer(EV3)
	command[14] = parsePort(port().name().at(0).toLatin1());
	mRobotCommunicator.send(this, command, 3);
}

void Motor::stop()
{
	on(0);
}

void Motor::off()
{
	QByteArray command(11, 0);
	command[0] = 9;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = 0x00;
	command[4] = DIRECT_COMMAND_NO_REPLY;
	const int globalVariablesCount = 0;
	const int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opOUTPUT_STOP;
	command[8] = LC0(0); //layer(EV3)
	command[9] = parsePort(port().name().at(0).toLatin1());
	command[10] = LC0(0x00);
	mRobotCommunicator.send(this, command, 3);
}

char Motor::parsePort(QChar portName)
{
	if (portName == 'A') {
		return 0x01;
	} else if (portName == 'B') {
		return 0x02;
	} else if (portName == 'C') {
		return 0x04;
	} else if (portName == 'D') {
		return 0x08;
	}
	return 0x00;
}


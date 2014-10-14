#include "motor.h"

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase;
using namespace robotModel;
using namespace utils;
using namespace robotCommunication;

Motor::Motor(DeviceInfo const &info, PortInfo const &port, RobotCommunicator &robotCommunicator)
	: Ev3Motor(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void Motor::on(int speed)
{
	//Start motor connected to port A with given speed.
	QByteArray command(15, 0);
	command[0] = 13;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = 0x00;
	command[4] = DIRECT_COMMAND_NO_REPLY;
	int globalVariablesCount = 0;
	int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opOUTPUT_POWER;
	command[8] = LC0(0); //layer(EV3)
	command[9] = outputPort(port().name().at(0).toLatin1());
	//LC1(speed)
	command[10] = (PRIMPAR_LONG  | PRIMPAR_CONST | PRIMPAR_1_BYTE);
	command[11] = speed & 0xFF;

	command[12] = opOUTPUT_START;
	command[13] = LC0(0); // layer(EV3)
	command[14] = outputPort(port().name().at(0).toLatin1());
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
	int globalVariablesCount = 0;
	int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opOUTPUT_STOP;
	command[8] = LC0(0); //layer
	command[9] = outputPort(port().name().at(0).toLatin1());
	command[10] = LC0(0x01); //Apply the brake at the end of the command
	mRobotCommunicator.send(this, command, 3);
}

///@todo: Сhange location
char Motor::outputPort(QChar portName)
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
	return ' ';
}


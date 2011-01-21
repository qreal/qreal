#include "motor.h"

using namespace qReal::interpreters::robots::details::robotParts;

// Mode
int const motorOn = 0x01;
int const brake = 0x02;
int const regulated = 0x04;

//Motor regulation
int const regulationModeIdle = 0x00;
int const regulationModeMotorSpeed = 0x01;
int const regulationModeMotorSync = 0x02;

//Motor run state
int const motorRunStateIdle = 0x00;
int const motorRunStateRampUp = 0x10;
int const motorRunStateRunning = 0x20;
int const motorRunStateRampDown = 0x40;

Motor::Motor(int const port, RobotCommunicationInterface *robotCommunicationInterface)
	: RobotPart(robotCommunicationInterface)
	, mPort(port)
{
}

void Motor::on(int speed)
{
	setOutputState(speed, brake | motorOn | regulated, regulationModeMotorSpeed, 100, motorRunStateRunning, 0);
}

void Motor::on(int speed, long unsigned int degrees)
{
	setOutputState(speed, brake | motorOn | regulated, regulationModeMotorSpeed, 100, motorRunStateRunning, degrees);
}

void Motor::stop()
{
	setOutputState(0, brake | motorOn | regulated, regulationModeMotorSpeed, 100, motorRunStateRunning, 0);
}

void Motor::off()
{
	setOutputState(0, regulated, regulationModeMotorSpeed, 100, motorRunStateIdle, 0);
}

void Motor::setOutputState(int speed, int mode, int regulation, int turnRatio, int runState, unsigned long tachoLimit)
{
	QByteArray command(15, 0);
	command[0] = 13;  // command length.
	command[1] = 0x00;

	command[2] = 0x80;  // reply not needed.

	command[3] = 0x04;  // SETOUTPUTSTATE direct command.

	command[4] = mPort;  // output port
	command[5] = speed;  // power set point (range: -100 -- 100)
	command[6] = mode;  // mode (bit field)

	command[7] = regulation;  // regulation
	command[8] = turnRatio;  // turn ratio

	command[9] = runState;  // run state

	command[10] = tachoLimit;  // TachoLimit
	command[11] = tachoLimit >> 8;  // TachoLimit
	command[12] = tachoLimit >> 16;  // TachoLimit
	command[13] = tachoLimit >> 24;  // TachoLimit
	command[14] = 0;                 // TachoLimit, suddenly
	mRobotCommunicationInterface->send(this, command, 3);
}

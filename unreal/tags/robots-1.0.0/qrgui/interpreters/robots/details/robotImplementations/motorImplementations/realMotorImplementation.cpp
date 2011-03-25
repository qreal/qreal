#include "realMotorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::motorImplementations;

RealMotorImplementation::RealMotorImplementation(int const port, RobotCommunicationInterface *robotCommunicationInterface)
	:  AbstractMotorImplementation(port)
	, mRobotCommunicationInterface(robotCommunicationInterface)
{
}

void RealMotorImplementation::on(int speed)
{
	on(speed, 0);
}

void RealMotorImplementation::on(int speed, long unsigned int degrees)
{
	setOutputState(speed, motorMode::MOTORON | motorMode::BRAKE | motorMode::REGULATED
			, regulationMode::REGULATION_MODE_MOTOR_SPEED, 100, runState::MOTOR_RUN_STATE_RUNNING, degrees);
}

void RealMotorImplementation::stop()
{
	setOutputState(0, motorMode::MOTORON | motorMode::BRAKE | motorMode::REGULATED
			, regulationMode::REGULATION_MODE_MOTOR_SPEED, 100, runState::MOTOR_RUN_STATE_RUNNING, 0);
}

void RealMotorImplementation::off()
{
	setOutputState(0, motorMode::REGULATED, regulationMode::REGULATION_MODE_MOTOR_SPEED
			, 100, runState::MOTOR_RUN_STATE_IDLE, 0);
}

void RealMotorImplementation::setOutputState(int speed, int mode
		, regulationMode::RegulationModeEnum regulation, int turnRatio, runState::RunStateEnum runState
		, unsigned long tachoLimit)
{
	QByteArray command(15, 0);
	command[0] = 13;  // command length.
	command[1] = 0x00;

	command[2] = telegramType::directCommandNoResponse;

	command[3] = commandCode::SETOUTPUTSTATE;

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

void RealMotorImplementation::resetMotorPosition(bool relative)
{
	QByteArray command(5, 0);
	command[0] = 3;  // command length.
	command[1] = 0x00;
	command[2] = telegramType::directCommandNoResponse;
	command[3] = commandCode::RESETMOTORPOSITION;
	command[4] = relative;
	mRobotCommunicationInterface->send(this, command, 3);
}

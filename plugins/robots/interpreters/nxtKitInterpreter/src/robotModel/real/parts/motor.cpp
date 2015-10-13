/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "motor.h"

using namespace nxt::robotModel::real::parts;
using namespace kitBase;
using namespace robotModel;
using namespace utils;
using namespace robotCommunication;

Motor::Motor(const DeviceInfo &info, const PortInfo &port, RobotCommunicator &robotCommunicator)
	: NxtMotor(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void Motor::on(int speed, bool breakMode)
{
	on(speed, 0, breakMode);
}

void Motor::on(int speed, long unsigned int degrees, bool breakMode)
{
	int mode = enums::motorMode::MOTORON | enums::motorMode::REGULATED;
	if (breakMode) {
		mode |= enums::motorMode::BRAKE;
	}
	setOutputState(speed, mode, enums::regulationMode::REGULATION_MODE_MOTOR_SPEED
			, 100, enums::runState::MOTOR_RUN_STATE_RUNNING, degrees);
}

void Motor::stop(bool breakMode)
{
	on(0, 0, breakMode);
}

void Motor::off()
{
	setOutputState(0, enums::motorMode::REGULATED, enums::regulationMode::REGULATION_MODE_MOTOR_SPEED
			, 100, enums::runState::MOTOR_RUN_STATE_IDLE, 0);
}

void Motor::setOutputState(int speed, int mode
		, enums::regulationMode::RegulationModeEnum regulation, int turnRatio
		, enums::runState::RunStateEnum runState, unsigned long tachoLimit)
{
	QByteArray command(15, 0);
	command[0] = 13;  // command length.
	command[1] = 0x00;

	command[2] = enums::telegramType::directCommandNoResponse;

	command[3] = enums::commandCode::SETOUTPUTSTATE;

	command[4] = port().name().at(0).toLatin1() - 'A';  // output port
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
	mRobotCommunicator.send(this, command, 3);
}

void Motor::resetMotorPosition(bool relative)
{
	QByteArray command(5, 0);
	command[0] = 3;  // command length.
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandNoResponse;
	command[3] = enums::commandCode::RESETMOTORPOSITION;
	command[4] = relative;
	mRobotCommunicator.send(this, command, 3);
}

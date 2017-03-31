/* Copyright 2016-2017 CyberTech Labs Ltd.
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

#include "trikKitInterpreterCommon/trikEmulation/trikmotoremu.h"

trik::TrikMotorEmu::TrikMotorEmu(kitBase::robotModel::robotParts::Motor *motor) : mMotor(motor)
{
}

void trik::TrikMotorEmu::setPower(int power, bool constrain)
{
	mMotor->on(constrain ? qBound(minControl(), power, maxControl()) : power);
}

int trik::TrikMotorEmu::power() const
{
	return mMotor->power();
}

void trik::TrikMotorEmu::powerOff()
{
	mMotor->off();
}

void trik::TrikMotorEmu::brake(int)
{
	powerOff(); // should be the same for 2D model, since there is no inertia
}

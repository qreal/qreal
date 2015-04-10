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

#include "nxtKit/robotModel/parts/nxtMotor.h"

using namespace nxt::robotModel::parts;
using namespace kitBase::robotModel;

NxtMotor::NxtMotor(const DeviceInfo &info, const PortInfo &port)
	: robotParts::Motor(info, port)
{
}

void NxtMotor::on(int speed)
{
	on(speed, true);
}

void NxtMotor::stop()
{
	stop(true);
}

void NxtMotor::off()
{
	stop(false);
}

void NxtMotor::on(int speed, bool breakMode)
{
	on(speed, 0, breakMode);
}

void NxtMotor::on(int speed, long unsigned int degrees, bool breakMode)
{
	robotParts::Motor::on(speed);
	Q_UNUSED(degrees);
	Q_UNUSED(breakMode);
}

void NxtMotor::stop(bool breakMode)
{
	Q_UNUSED(breakMode);
}

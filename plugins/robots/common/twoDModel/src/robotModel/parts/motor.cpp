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

#include "twoDModel/robotModel/parts/motor.h"

#include "twoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace kitBase;
using namespace robotModel;

Motor::Motor(const DeviceInfo &info, const PortInfo &port, engine::TwoDModelEngineInterface &engine)
	: kitBase::robotModel::robotParts::Motor(info, port)
	, mEngine(engine)
{
}

void Motor::on(int speed)
{
	on(speed, true);
}

void Motor::stop()
{
	stop(true);
}

void Motor::on(int speed, bool breakMode)
{
	on(speed, 0, breakMode);
}

void Motor::on(int speed, long unsigned int degrees, bool breakMode)
{
	kitBase::robotModel::robotParts::Motor::on(speed);
	mEngine.setNewMotor(speed, degrees, port(), breakMode);
}

void Motor::stop(bool breakMode)
{
	on(0, 0, breakMode);
}

void Motor::off()
{
	stop(false);
}

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

#include "twoDMotor.h"

using namespace ev3::robotModel::twoD::parts;

TwoDMotor::TwoDMotor(kitBase::robotModel::DeviceInfo const &info
		, kitBase::robotModel::PortInfo const &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: Ev3Motor(info, port)
	, mEngine(engine)
{
}

void TwoDMotor::on(int speed)
{
	mEngine.setNewMotor(speed, 0, port(), true);
}

void TwoDMotor::stop()
{
	on(0);
}

void TwoDMotor::off()
{
	on(0);
}

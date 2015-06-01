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

using namespace nxt::robotModel::twoD::parts;

TwoDMotor::TwoDMotor(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: NxtMotor(info, port)
	, mEngine(engine)
{
}

void TwoDMotor::on(int speed, unsigned long degrees, bool breakMode)
{
	NxtMotor::on(speed, degrees, breakMode);
	mEngine.setNewMotor(speed, degrees, port(), breakMode);
}

void TwoDMotor::stop(bool breakMode)
{
	on(0, 0, breakMode);
}

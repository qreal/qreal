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

#include "speaker.h"
#include "commandConstants.h"

using namespace nxt::robotModel::real::parts;
using namespace kitBase::robotModel;

Speaker::Speaker(const DeviceInfo &info, const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: robotModel::parts::NxtSpeaker(info, port)
	, mRobotCommunicator(robotCommunicator)
{
}

void Speaker::playTone(unsigned freq, unsigned time)
{
	QByteArray command(8, 0);
	command[0] = 0x06;  //command length
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandNoResponse;
	command[3] = enums::commandCode::PLAYTONE;
	command[4] = freq;
	command[5] = freq >> 8;
	command[6] = time;
	command[7] = time >> 8;
	mRobotCommunicator.send(this, command, 5);
}

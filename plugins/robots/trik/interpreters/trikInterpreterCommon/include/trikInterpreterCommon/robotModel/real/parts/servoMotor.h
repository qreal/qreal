/* Copyright 2014-2016 CyberTech Labs Ltd.
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

#pragma once

#include <trikCommon/robotModel/parts/trikServoMotor.h>
#include <trikCommon/robotCommunication/tcpRobotCommunicator.h>

#include "trikInterpreterCommon/declSpec.h"

namespace trik {
namespace robotModel {
namespace real {
namespace parts {

class ROBOTS_TRIK_KIT_INTERPRETER_COMMON_EXPORT ServoMotor : public robotModel::parts::TrikServoMotor
{
	Q_OBJECT

public:
	ServoMotor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, robotCommunication::TcpRobotCommunicator &tcpRobotCommunicator);

	void on(int speed) override;
	void stop() override;
	void off() override;

private:
	robotCommunication::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}

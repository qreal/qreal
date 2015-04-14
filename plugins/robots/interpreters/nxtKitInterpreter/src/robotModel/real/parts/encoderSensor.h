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

#pragma once

#include <kitBase/robotModel/robotParts/encoderSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

class EncoderSensor : public kitBase::robotModel::robotParts::EncoderSensor
{
	Q_OBJECT

public:
	EncoderSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void read() override;
	void nullify() override;

private slots:
	void readingDone(QObject *addressee, const QByteArray &reading);

private:
	enum State {
		idle
		, pending
	};

	void sensorSpecificProcessResponse(const QByteArray &reading);

	char lowLevelPort() const;

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
	State mState;
};

}
}
}
}

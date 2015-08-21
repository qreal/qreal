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

#include <QtCore/QByteArray>

#include <kitBase/robotModel/portInfo.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace nxt {
namespace communication {

class I2CCommunicator
{
public:
	explicit I2CCommunicator(utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void sendI2C(QObject *addressee, const QByteArray &buffer
			, const int responseSize
			, const kitBase::robotModel::PortInfo &port);

private:
	static const int i2cTimeout = 5000;

	bool waitForI2CBytes(int bytes, const kitBase::robotModel::PortInfo &port);
	int i2cBytesReady(const kitBase::robotModel::PortInfo &port);

	char toNxtInputPort(const kitBase::robotModel::PortInfo &port);

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}

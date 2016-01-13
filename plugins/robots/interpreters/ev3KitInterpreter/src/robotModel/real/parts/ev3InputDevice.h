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

#include <kitBase/robotModel/robotParts/abstractSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace ev3 {
namespace robotModel {
namespace real {
namespace parts {

class Ev3InputDevice : public QObject
{
	Q_OBJECT

public:
	Ev3InputDevice(utils::robotCommunication::RobotCommunicator &robotCommunicator
			, const kitBase::robotModel::PortInfo &port);

	void send(QByteArray command, const unsigned responseSize, QByteArray reading);

	/// Returns a value of port that can be used as corresponding byte in request packages.
	char lowLevelPort() const;

	QByteArray readySiCommand(int port, int sensorMode);
	QByteArray readyRawCommand(int port, int sensorMode);
	QByteArray readyPercentCommand(int port, int sensorMode);

private:
	//enums::inputPort::InputPortEnum parsePort();

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
	char mLowLevelPort;
};

}
}
}
}

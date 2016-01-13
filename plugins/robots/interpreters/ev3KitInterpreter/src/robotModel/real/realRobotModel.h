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

#include <ev3Kit/robotModel/ev3RobotModelBase.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace ev3 {
namespace robotModel {
namespace real {

class RealRobotModel : public Ev3RobotModelBase
{
	Q_OBJECT

public:
	RealRobotModel(const QString &kitId, const QString &robotId
			, utils::robotCommunication::RobotCommunicationThreadInterface *communicationThread);

	bool needsConnection() const override;

	void connectToRobot() override;
	void disconnectFromRobot() override;

	/// Checks if connection can be established or emits errorOccured();
	void checkConnection();

signals:
	/// Emitted when communicator throws an error to be displayed with error reporter.
	void errorOccured(const QString &text);

private:
	kitBase::robotModel::robotParts::Device *createDevice(
			const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &deviceInfo) override;

	// WARNING: This class must be disposed in the last turn so do not make it storing by value.
	utils::robotCommunication::RobotCommunicator *mRobotCommunicator;  // Takes ownership
	QString mLastCommunicationValue;
};

}
}
}

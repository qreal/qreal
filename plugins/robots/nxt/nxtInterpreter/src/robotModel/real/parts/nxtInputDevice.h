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

#include <kitBase/robotModel/robotParts/abstractSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "commandConstants.h"

namespace nxt {
namespace robotModel {
namespace real {
namespace parts {

/// Implements some base methods for working with real nxt devices. This class must be embedded into the sensor
/// and sensorSpecificProcessResponse() and configured() signals must be connected.
class NxtInputDevice : public QObject
{
	Q_OBJECT

public:
	/// Represents a state of the communication with device.
	enum State {
		idle
		, pending
	};

	NxtInputDevice(
			utils::robotCommunication::RobotCommunicator &robotCommunicator
			, const kitBase::robotModel::PortInfo &port
			, const enums::lowLevelSensorType::SensorTypeEnum &lowLevelSensorType
			, const enums::sensorMode::SensorModeEnum &sensorMode);

	/// Sends configuration buffer into the real device. When configuration is done configured() singal is emitted.
	void configure();

	/// Sends given buffer to a real device.
	/// @param responseSize An expected size of the responce buffer.
	void send(const QByteArray &buffer, const unsigned responseSize);

	/// Returns a state of the communication with the device.
	State state() const;

	/// Sets a state of the communication with the device.
	void setState(State state);

	/// Returns configuration state of the device.
	bool isConfigured() const;

	/// Returns a value of port that can be used as corresponding byte in request packages.
	char lowLevelPort() const;

signals:
	/// Emitted when responce buffer has unknown data and must be processed by the concrete device.
	void sensorSpecificProcessResponse(const QByteArray &reading);

	/// Emitted when the configuration process is done.
	void configured(bool success);

private slots:
	void readingDone(QObject *addressee, const QByteArray &reading);

private:
	void processResponse(const QByteArray &reading);

	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
	char mLowLevelPort;
	enums::lowLevelSensorType::SensorTypeEnum mLowLevelSensorType;
	enums::sensorMode::SensorModeEnum mSensorMode;
	State mState;
	bool mIsConfigured;
	bool mResetDone;
};

}
}
}
}

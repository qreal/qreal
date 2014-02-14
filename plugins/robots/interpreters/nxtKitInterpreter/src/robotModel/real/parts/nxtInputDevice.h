#pragma once

#include <interpreterBase/robotModel/robotParts/abstractSensor.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "commandConstants.h"

namespace nxtKitInterpreter {
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

	/// @param implementation - concrete sensor instance for binding with it sent buffers and responses to them
	NxtInputDevice(interpreterBase::robotModel::robotParts::AbstractSensor const &sensor
			,utils::robotCommunication::RobotCommunicator &robotCommunicator
			, interpreterBase::robotModel::PortInfo const &port
			, enums::lowLevelSensorType::SensorTypeEnum const &lowLevelSensorType
			, enums::sensorMode::SensorModeEnum const &sensorMode);

	/// Sends configuration buffer into the real device. When configuration is done configured() singal is emitted.
	void configure();

	/// Sends given buffer to a real device.
	/// @param responseSize An expected size of the responce buffer.
	void send(QByteArray const &buffer, unsigned const responseSize);

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
	void sensorSpecificProcessResponse(QByteArray const &reading);

	/// Emitted when the configuration process is done.
	void configured(bool success);

private slots:
	void readingDone(QObject *addressee, QByteArray const &reading);

private:
	void processResponse(QByteArray const &reading);

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

#pragma once

#include <interpreterBase/robotModel/portInfo.h>
#include <utils/robotCommunication/robotCommunicator.h>

#include "commandConstants.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

/// A mixin class impelementing some base methods for working with real nxt devices
class NxtInputDevice
{
public:
	/// @param implementation - concrete sensor instance for binding with it sent buffers and responses to them
	NxtInputDevice(QObject *instance
			,utils::robotCommunication::RobotCommunicator &robotCommunicator
			, interpreterBase::robotModel::PortInfo const &port
			, enums::lowLevelSensorType::SensorTypeEnum const &lowLevelSensorType
			, enums::sensorMode::SensorModeEnum const &sensorMode);

protected:
	virtual void configureNxtDevice();
	virtual void processResponse(QByteArray const &reading);

	virtual void sensorSpecificProcessResponse(QByteArray const &reading) = 0;
	virtual void onDeviceConfigured();

private slots:
	void readingDone(QObject *addressee, QByteArray const &reading);

protected:
	enum State {
		idle
		, pending
	};

	QObject *mInstance;
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

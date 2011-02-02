#pragma once

#include <QtCore/QObject>

#include "robotPart.h"
#include "../robotCommandConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class Sensor : public RobotPart
{
	Q_OBJECT

public:
	Sensor(RobotCommunicationInterface *robotCommunicationInterface
			, lowLevelSensorType::SensorTypeEnum const &lowLevelSensorType
			, sensorMode::SensorModeEnum const &sensorMode
			, lowLevelInputPort::InputPortEnum const &port
			);
	void configure();
	virtual void read();

signals:
	void response(int reading);
	void failure();
	void configured();

protected:
	enum State {
		idle
		, pending
	};

	lowLevelInputPort::InputPortEnum mPort;
	State mState;

private:
	lowLevelSensorType::SensorTypeEnum mSensorType;
	sensorMode::SensorModeEnum mSensorMode;
	bool mIsConfigured;
	bool mResetDone;

	virtual void processResponse(QByteArray const &reading);
	virtual void sensorSpecificProcessResponse(QByteArray const &reading) = 0;
};

}
}
}
}
}

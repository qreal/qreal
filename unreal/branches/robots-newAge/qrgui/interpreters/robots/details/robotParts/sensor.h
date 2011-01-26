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
	void read();

signals:
	void response(int reading);
	void fail();
	void configured();

private:
	enum State {
		idle
		, pending
	};

	State mState;
	lowLevelInputPort::InputPortEnum mPort;
	lowLevelSensorType::SensorTypeEnum mSensorType;
	sensorMode::SensorModeEnum mSensorMode;
	bool mIsConfigured;

	virtual void processResponse(QByteArray const &reading);
};

}
}
}
}
}

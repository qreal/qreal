#pragma once

#include <QtCore/QObject>

#include "robotPart.h"
#include "../../sensorType.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class Sensor : public RobotPart
{
	Q_OBJECT

public:
	Sensor(RobotCommunicationInterface *robotCommunicationInterface, SensorType::SensorType const &sensorType, int port);
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
	int mPort;
	SensorType::SensorType mSensorType;
	bool mIsConfigured;

	virtual void processResponse(QByteArray const &reading);
};

}
}
}
}
}

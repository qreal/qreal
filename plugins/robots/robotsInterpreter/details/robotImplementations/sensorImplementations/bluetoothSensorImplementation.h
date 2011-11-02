#pragma once
#include "abstractSensorImplementation.h"
#include "../../robotCommandConstants.h"
#include "../../robotCommunication/robotCommunicationThreadInterface.h"
#include "../../robotCommunication/robotCommunication.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class BluetoothSensorImplementation : public AbstractSensorImplementation
{
	Q_OBJECT

public:
	BluetoothSensorImplementation(RobotCommunication *robotCommunicationInterface
		, sensorType::SensorTypeEnum const &sensorType
		, lowLevelSensorType::SensorTypeEnum const &lowLevelSensorType
		, sensorMode::SensorModeEnum const &sensorMode
		, inputPort::InputPortEnum const &port
		);
	virtual ~BluetoothSensorImplementation();
	virtual void configure();
	virtual void read() = 0;

protected slots:
	void readingDone(QObject *addressee, QByteArray const &reading);

protected:
	RobotCommunication *mRobotCommunicationInterface;
	lowLevelSensorType::SensorTypeEnum mLowLevelSensorType;
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
}

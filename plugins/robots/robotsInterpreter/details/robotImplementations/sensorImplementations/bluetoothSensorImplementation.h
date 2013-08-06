#pragma once
#include "abstractSensorImplementation.h"
#include "../../robotCommandConstants.h"
#include "../../robotCommunication/robotCommunicationThreadInterface.h"
#include "../../robotCommunication/robotCommunicator.h"

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
	BluetoothSensorImplementation(RobotCommunicator *robotCommunicationInterface
		, robots::enums::sensorType::SensorTypeEnum const &sensorType
		, enums::lowLevelSensorType::SensorTypeEnum const &lowLevelSensorType
		, enums::sensorMode::SensorModeEnum const &sensorMode
		, robots::enums::inputPort::InputPortEnum const port
		);
	virtual ~BluetoothSensorImplementation();
	virtual void configure();
	virtual void read() = 0;

protected slots:
	void readingDone(QObject *addressee, QByteArray const &reading);

protected:
	RobotCommunicator *mRobotCommunicationInterface;
	enums::lowLevelSensorType::SensorTypeEnum mLowLevelSensorType;
	enums::sensorMode::SensorModeEnum mSensorMode;
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

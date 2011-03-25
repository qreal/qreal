#pragma once

#include "bluetoothSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class BluetoothColorSensorImplementation : public BluetoothSensorImplementation
{
	Q_OBJECT

public:
	BluetoothColorSensorImplementation(RobotCommunicationInterface *robotCommunicationInterface
			, inputPort::InputPortEnum const &port, lowLevelSensorType::SensorTypeEnum mode);
	virtual void read();
	void reconfigure(lowLevelSensorType::SensorTypeEnum mode);

protected:
	virtual void sensorSpecificProcessResponse(QByteArray const &reading);
};

}
}
}
}
}
}

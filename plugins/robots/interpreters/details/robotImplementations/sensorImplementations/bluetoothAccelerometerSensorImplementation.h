#pragma once

#include "bluetoothSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class BluetoothAccelerometerSensorImplementation : public BluetoothSensorImplementation
{
	Q_OBJECT

public:
	BluetoothAccelerometerSensorImplementation(RobotCommunicator *robotCommunicationInterface
			, robots::enums::inputPort::InputPortEnum port);
	virtual void read();

protected:
	virtual void sensorSpecificProcessResponse(QByteArray const &reading);
};

}
}
}
}
}
}

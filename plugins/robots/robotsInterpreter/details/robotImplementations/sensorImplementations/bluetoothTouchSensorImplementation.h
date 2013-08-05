#pragma once
#include "bluetoothSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class BluetoothTouchSensorImplementation : public BluetoothSensorImplementation
{
	Q_OBJECT

public:
	BluetoothTouchSensorImplementation(
			RobotCommunicator *robotCommunicationInterface
			, robots::enums::inputPort::InputPortEnum const port
			);

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

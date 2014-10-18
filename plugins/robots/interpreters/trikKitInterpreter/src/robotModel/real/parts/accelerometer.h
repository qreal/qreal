#pragma once

#include <interpreterBase/robotModel/robotParts/accelerometerSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Accelerometer : public interpreterBase::robotModel::robotParts::AccelerometerSensor
{
	Q_OBJECT

public:
	Accelerometer(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void read() override;
};

}
}
}
}

#pragma once

#include <interpreterBase/robotModel/robotParts/gyroscopeSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Gyroscope : public interpreterBase::robotModel::robotParts::GyroscopeSensor
{
	Q_OBJECT

public:
	Gyroscope(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void read() override;
};

}
}
}
}

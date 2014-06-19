#pragma once

#include <interpreterBase/robotModel/robotParts/lightSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class LightSensor : public interpreterBase::robotModel::robotParts::LightSensor
{
	Q_OBJECT

public:
	LightSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void read() override;
};

}
}
}
}

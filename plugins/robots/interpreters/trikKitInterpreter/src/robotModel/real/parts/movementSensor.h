#pragma once

#include "robotModel/parts/trikMovementSensor.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class MovementSensor : public robotModel::parts::TrikMovementSensor
{
	Q_OBJECT

public:
	MovementSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void read() override;
};

}
}
}
}

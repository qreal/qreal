#pragma once

#include "robotModel/parts/trikMotionSensor.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class MotionSensor : public robotModel::parts::TrikMotionSensor
{
	Q_OBJECT

public:
	MotionSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void read() override;
};

}
}
}
}

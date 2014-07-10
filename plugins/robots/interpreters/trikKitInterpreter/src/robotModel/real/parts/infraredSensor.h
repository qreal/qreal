#pragma once

#include "robotModel/parts/trikInfraredSensor.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class InfraredSensor : public robotModel::parts::TrikInfraredSensor
{
	Q_OBJECT

public:
	InfraredSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void read() override;
};

}
}
}
}

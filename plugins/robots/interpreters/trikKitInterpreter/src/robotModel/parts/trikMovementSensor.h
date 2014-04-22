#pragma once

#include <interpreterBase/robotModel/robotParts/scalarSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

class TrikMovementSensor : public interpreterBase::robotModel::robotParts::ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Movement Sensor"))

public:
	TrikMovementSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

private:
	void doConfiguration() override;
};

}
}
}

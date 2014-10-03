#pragma once

#include <interpreterBase/robotModel/robotParts/scalarSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

class TrikMotionSensor : public interpreterBase::robotModel::robotParts::ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "motion")
	Q_CLASSINFO("friendlyName", tr("Motion Sensor"))

public:
	TrikMotionSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);
};

}
}
}

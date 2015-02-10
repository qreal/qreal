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
	TrikMotionSensor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);
};

}
}
}

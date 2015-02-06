#pragma once

#include <interpreterBase/robotModel/robotParts/rangeSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

class TrikSonarSensor : public interpreterBase::robotModel::robotParts::RangeSensor
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Sonic Sensor"))

public:
	TrikSonarSensor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);
};

}
}
}

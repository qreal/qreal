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
	TrikSonarSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);
};

}
}
}

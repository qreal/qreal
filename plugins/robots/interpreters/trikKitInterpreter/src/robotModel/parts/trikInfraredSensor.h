#pragma once

#include <interpreterBase/robotModel/robotParts/rangeSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

class TrikInfraredSensor : public interpreterBase::robotModel::robotParts::RangeSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "infrared")
	Q_CLASSINFO("friendlyName", tr("Infrared Sensor"))

public:
	TrikInfraredSensor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);
};

}
}
}

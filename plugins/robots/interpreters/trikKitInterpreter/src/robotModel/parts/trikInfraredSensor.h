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
	TrikInfraredSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

private:
	void doConfiguration() override;
};

}
}
}

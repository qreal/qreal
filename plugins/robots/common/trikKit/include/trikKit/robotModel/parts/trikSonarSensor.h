#pragma once

#include <kitBase/robotModel/robotParts/rangeSensor.h>

namespace trik {
namespace robotModel {
namespace parts {

class TrikSonarSensor : public kitBase::robotModel::robotParts::RangeSensor
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Sonic Sensor"))

public:
	TrikSonarSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);
};

}
}
}

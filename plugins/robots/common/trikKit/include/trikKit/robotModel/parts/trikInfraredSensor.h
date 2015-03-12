#pragma once

#include <kitBase/robotModel/robotParts/rangeSensor.h>

namespace trik {
namespace robotModel {
namespace parts {

class TrikInfraredSensor : public kitBase::robotModel::robotParts::RangeSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "infrared")
	Q_CLASSINFO("friendlyName", tr("Infrared Sensor"))

public:
	TrikInfraredSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);
};

}
}
}

#pragma once

#include <kitBase/robotModel/robotParts/scalarSensor.h>

namespace trik {
namespace robotModel {
namespace parts {

class TrikMotionSensor : public kitBase::robotModel::robotParts::ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "motion")
	Q_CLASSINFO("friendlyName", tr("Motion Sensor"))

public:
	TrikMotionSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);
};

}
}
}

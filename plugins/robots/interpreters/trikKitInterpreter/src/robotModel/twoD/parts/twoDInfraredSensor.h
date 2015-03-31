#pragma once

#include <twoDModel/robotModel/parts/rangeSensor.h>

namespace trik {
namespace robotModel {
namespace twoD {
namespace parts {

class TwoDInfraredSensor : public twoDModel::robotModel::parts::RangeSensor
{
	Q_OBJECT

public:
	TwoDInfraredSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void read() override;
};

}
}
}
}

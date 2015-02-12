#pragma once

#include <commonTwoDModel/robotModel/parts/rangeSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {

class TwoDInfraredSensor : public twoDModel::robotModel::parts::RangeSensor
{
	Q_OBJECT

public:
	TwoDInfraredSensor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void read() override;
};

}
}
}
}

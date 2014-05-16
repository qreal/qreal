#pragma once

#include <commonTwoDModel/robotModel/parts/rangeSensor.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {

class COMMON_TWO_D_MODEL_EXPORT TwoDInfraredSensor : public twoDModel::robotModel::parts::RangeSensor
{
	Q_OBJECT

public:
	TwoDInfraredSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void read() override;
};

}
}
}
}

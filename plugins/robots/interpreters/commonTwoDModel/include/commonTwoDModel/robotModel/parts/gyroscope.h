#pragma once

#include <interpreterBase/robotModel/robotParts/gyroscopeSensor.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// A mock up of the gyroscope sensor for the 2D model window. Retuns empty answers.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class COMMON_TWO_D_MODEL_EXPORT Gyroscope : public interpreterBase::robotModel::robotParts::GyroscopeSensor
{
	Q_OBJECT

public:
	Gyroscope(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, engine::TwoDModelEngineInterface &engine);

	void read() override;

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

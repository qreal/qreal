#pragma once

#include <interpreterBase/robotModel/robotParts/lightSensor.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the light sensor for the 2D model window.
/// Returns a brightness value from 0 to 100.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class COMMON_TWO_D_MODEL_EXPORT LightSensor : public interpreterBase::robotModel::robotParts::LightSensor
{
	Q_OBJECT

public:
	LightSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, engine::TwoDModelEngineInterface &engine);

	void read() override;

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

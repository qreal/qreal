#pragma once

#include <interpreterBase/robotModel/robotParts/rangeSensor.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the range sensor for the 2D model window.
/// Returns a distance in cm from 0 to 255.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class COMMON_TWO_D_MODEL_EXPORT RangeSensor : public interpreterBase::robotModel::robotParts::RangeSensor
{
	Q_OBJECT

public:
	RangeSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, engine::TwoDModelEngineInterface &engine);

	void read() override;

protected:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

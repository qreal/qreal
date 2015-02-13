#pragma once

#include <interpreterBase/robotModel/robotParts/encoderSensor.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the encoder sensor for the 2D model window.
/// Returns an encoder value in degrees.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class COMMON_TWO_D_MODEL_EXPORT EncoderSensor : public interpreterBase::robotModel::robotParts::EncoderSensor
{
	Q_OBJECT

public:
	EncoderSensor(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	void read() override;
	void nullify() override;

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

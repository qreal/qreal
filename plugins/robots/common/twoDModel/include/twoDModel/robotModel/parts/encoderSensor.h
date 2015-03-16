#pragma once

#include <kitBase/robotModel/robotParts/encoderSensor.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the encoder sensor for the 2D model window.
/// Returns an encoder value in degrees.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class TWO_D_MODEL_EXPORT EncoderSensor : public kitBase::robotModel::robotParts::EncoderSensor
{
	Q_OBJECT

public:
	EncoderSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	void read() override;
	void nullify() override;

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

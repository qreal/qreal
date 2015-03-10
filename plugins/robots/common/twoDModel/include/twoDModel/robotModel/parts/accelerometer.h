#pragma once

#include <kitBase/robotModel/robotParts/accelerometerSensor.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// A mock up of the accelerometer sensor for the 2D model window. Retuns empty answers.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class TWO_D_MODEL_EXPORT Accelerometer : public kitBase::robotModel::robotParts::AccelerometerSensor
{
	Q_OBJECT

public:
	Accelerometer(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	void read() override;

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

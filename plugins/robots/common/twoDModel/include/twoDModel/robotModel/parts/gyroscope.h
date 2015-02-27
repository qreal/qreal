#pragma once

#include <kitBase/robotModel/robotParts/gyroscopeSensor.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// A mock up of the gyroscope sensor for the 2D model window. Retuns empty answers.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class TWO_D_MODEL_EXPORT Gyroscope : public kitBase::robotModel::robotParts::GyroscopeSensor
{
	Q_OBJECT

public:
	Gyroscope(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	void read() override;

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

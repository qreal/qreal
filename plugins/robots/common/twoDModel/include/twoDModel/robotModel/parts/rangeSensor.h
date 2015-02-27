#pragma once

#include <kitBase/robotModel/robotParts/rangeSensor.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the range sensor for the 2D model window.
/// Returns a distance in cm from 0 to 255.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class TWO_D_MODEL_EXPORT RangeSensor : public kitBase::robotModel::robotParts::RangeSensor
{
	Q_OBJECT

public:
	RangeSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	void read() override;

protected:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

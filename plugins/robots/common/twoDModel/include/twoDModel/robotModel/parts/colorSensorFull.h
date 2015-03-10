#pragma once

#include <kitBase/robotModel/robotParts/colorSensorFull.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the color sensor in the color recognition mode for the 2D model window.
/// Returns a recognized color enum value under this color sensor.
/// Configuration is perfomed immediately, the answer is ready immediately too.
/// @todo: Return a enum value really, not some secret hardcoded value.
class TWO_D_MODEL_EXPORT ColorSensorFull : public kitBase::robotModel::robotParts::ColorSensorFull
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Color sensor (full)"))

public:
	ColorSensorFull(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	void read();

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

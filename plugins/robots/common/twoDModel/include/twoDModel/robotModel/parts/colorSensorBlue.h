#pragma once

#include <kitBase/robotModel/robotParts/colorSensorBlue.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the color sensor in the blue color mode for the 2D model window.
/// Returns a blue color percent in the picture under the sensor: a value from 0 to 100.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class TWO_D_MODEL_EXPORT ColorSensorBlue : public kitBase::robotModel::robotParts::ColorSensorBlue
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Color sensor (blue)"))

public:
	ColorSensorBlue(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	void read();

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

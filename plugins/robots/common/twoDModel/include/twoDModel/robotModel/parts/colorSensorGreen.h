#pragma once

#include <kitBase/robotModel/robotParts/colorSensorGreen.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the color sensor in the green color mode for the 2D model window.
/// Returns a green color percent in the picture under the sensor: a value from 0 to 100.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class TWO_D_MODEL_EXPORT ColorSensorGreen : public kitBase::robotModel::robotParts::ColorSensorGreen
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Color sensor (green)"))

public:
	ColorSensorGreen(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	void read();

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

#pragma once

#include <interpreterBase/robotModel/robotParts/colorSensorGreen.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the color sensor in the green color mode for the 2D model window.
/// Returns a green color percent in the picture under the sensor: a value from 0 to 100.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class COMMON_TWO_D_MODEL_EXPORT ColorSensorGreen : public interpreterBase::robotModel::robotParts::ColorSensorGreen
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Color sensor (green)"))

public:
	ColorSensorGreen(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, engine::TwoDModelEngineInterface &engine);

	void read();

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

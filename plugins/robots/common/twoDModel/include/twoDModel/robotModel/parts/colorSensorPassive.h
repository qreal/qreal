#pragma once

#include <kitBase/robotModel/robotParts/colorSensorPassive.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the color sensor in the passive mode
/// (when the sensor doesn`t light, only accepts it) for the 2D model window.
/// Returns a brightness in the picture under the sensor: a value from 0 to 100.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class TWO_D_MODEL_EXPORT ColorSensorPassive : public kitBase::robotModel::robotParts::ColorSensorPassive
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Color sensor (passive)"))

public:
	ColorSensorPassive(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	void read();

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

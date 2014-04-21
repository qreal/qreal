#pragma once

#include <interpreterBase/robotModel/robotParts/colorSensorRed.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

class COMMON_TWO_D_MODEL_EXPORT ColorSensorRed : public interpreterBase::robotModel::robotParts::ColorSensorRed
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Color sensor (red)"))

public:
	ColorSensorRed(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, engine::TwoDModelEngineInterface &engine);

	void read();

protected:
	void doConfiguration();

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

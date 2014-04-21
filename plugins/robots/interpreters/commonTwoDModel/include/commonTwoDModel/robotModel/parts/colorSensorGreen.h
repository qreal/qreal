#pragma once

#include <interpreterBase/robotModel/robotParts/colorSensorGreen.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

class COMMON_TWO_D_MODEL_EXPORT ColorSensorGreen : public interpreterBase::robotModel::robotParts::ColorSensorGreen
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Color sensor (green)"))

public:
	ColorSensorGreen(interpreterBase::robotModel::DeviceInfo const &info
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

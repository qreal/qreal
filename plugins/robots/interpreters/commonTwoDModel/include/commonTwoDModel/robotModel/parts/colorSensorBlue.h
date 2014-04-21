#pragma once

#include <interpreterBase/robotModel/robotParts/colorSensorBlue.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

class COMMON_TWO_D_MODEL_EXPORT ColorSensorBlue : public interpreterBase::robotModel::robotParts::ColorSensorBlue
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Color sensor (blue)"))

public:
	ColorSensorBlue(interpreterBase::robotModel::DeviceInfo const &info
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

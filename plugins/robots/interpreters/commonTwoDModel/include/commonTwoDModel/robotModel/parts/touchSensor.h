#pragma once

#include <interpreterBase/robotModel/robotParts/touchSensor.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

class COMMON_TWO_D_MODEL_EXPORT TouchSensor : public interpreterBase::robotModel::robotParts::TouchSensor
{
	Q_OBJECT

public:
	TouchSensor(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, engine::TwoDModelEngineInterface &engine);

	void read() override;

protected:
	void doConfiguration() override;

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

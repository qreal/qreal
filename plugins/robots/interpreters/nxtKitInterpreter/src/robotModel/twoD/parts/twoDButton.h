#pragma once

#include "robotModel/parts/nxtButton.h"

#include <commonTwoDModel/engine/twoDModelEngineInterface.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {

class TwoDButtons : public robotModel::parts::NxtButton
{
	Q_OBJECT

public:
	TwoDButtons(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void read() override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
};

}
}
}
}

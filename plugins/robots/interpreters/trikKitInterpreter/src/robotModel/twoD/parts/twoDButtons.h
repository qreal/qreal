#pragma once

#include "robotModel/parts/trikButtons.h"

#include <commonTwoDModel/engine/twoDModelEngineInterface.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {

class TwoDButtons : public robotModel::parts::TrikButtons
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

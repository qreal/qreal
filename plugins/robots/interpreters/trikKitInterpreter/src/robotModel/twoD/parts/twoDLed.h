#pragma once

#include "robotModel/parts/trikLed.h"

#include <commonTwoDModel/engine/twoDModelEngineInterface.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {


class TwoDLed : public robotModel::parts::TrikLed
{
	Q_OBJECT

public:
	TwoDLed(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void setColor(QColor const &color) override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
};


}
}
}
}


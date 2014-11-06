#pragma once

#include <commonTwoDModel/engine/twoDModelEngineInterface.h>

#include "robotModel/parts/trikLed.h"

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

	/// sets LED widget's color in 2D model.
	void setColor(QString const &color) override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
};

}
}
}
}

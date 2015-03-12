#pragma once

#include "robotModel/parts/nxtMotor.h"

#include <commonTwoDModel/engine/twoDModelEngineInterface.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {

class TwoDMotor : public robotModel::parts::NxtMotor
{
public:
	TwoDMotor(kitBase::robotModel::DeviceInfo const &info
			, kitBase::robotModel::PortInfo const &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void on(int speed, long unsigned int degrees, bool breakMode) override;
	void stop(bool breakMode) override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
};

}
}
}
}

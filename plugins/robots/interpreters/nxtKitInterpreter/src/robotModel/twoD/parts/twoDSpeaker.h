#pragma once

#include "robotModel/parts/nxtSpeaker.h"

#include <commonTwoDModel/engine/twoDModelEngineInterface.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {

class TwoDSpeaker : public robotModel::parts::NxtSpeaker
{
	Q_OBJECT

public:
	TwoDSpeaker(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void playTone(unsigned freq, unsigned time) override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
};

}
}
}
}

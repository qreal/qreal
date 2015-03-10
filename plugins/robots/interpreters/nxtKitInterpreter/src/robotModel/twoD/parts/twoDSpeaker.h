#pragma once

#include <nxtKit/robotModel/parts/nxtSpeaker.h>

#include <twoDModel/engine/twoDModelEngineInterface.h>

namespace nxt {
namespace robotModel {
namespace twoD {
namespace parts {

class TwoDSpeaker : public robotModel::parts::NxtSpeaker
{
	Q_OBJECT

public:
	TwoDSpeaker(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void playTone(unsigned freq, unsigned time) override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
};

}
}
}
}

#pragma once

#include <nxtKit/robotModel/parts/nxtMotor.h>

#include <twoDModel/engine/twoDModelEngineInterface.h>

namespace nxt {
namespace robotModel {
namespace twoD {
namespace parts {

class TwoDMotor : public robotModel::parts::NxtMotor
{
public:
	TwoDMotor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
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

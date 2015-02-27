#pragma once

#include <kitBase/robotModel/portInfo.h>
#include <kitBase/robotModel/robotParts/motor.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the motor for the 2D model window. Supports break modes.
/// Configuration is perfomed immediately, the answer is ready immediately too.
/// @todo Do something with copypaste with real NXT motor.
class TWO_D_MODEL_EXPORT Motor : public kitBase::robotModel::robotParts::Motor
{
	Q_OBJECT

public:
	Motor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine
			);

	void on(int speed) override;
	void stop() override;
	void off() override;

	void on(int speed, bool breakMode);
	void on(int speed, long unsigned int degrees, bool breakMode);
	void stop(bool breakMode);

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

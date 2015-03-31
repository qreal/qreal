#pragma once

#include <kitBase/robotModel/robotParts/button.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the button sensor for the 2D model display emulator.
/// Returns if button is pressed and 0 otherwise.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class Button : public kitBase::robotModel::robotParts::Button
{
	Q_OBJECT

public:
	Button(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	void read() override;

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

#pragma once

#include <interpreterBase/robotModel/robotParts/button.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the button sensor for the 2D model display emulator.
/// Returns if button is pressed and 0 otherwise.
/// Configuration is perfomed immediately, the answer is ready immediately too.
class Button : public interpreterBase::robotModel::robotParts::Button
{
	Q_OBJECT

public:
	Button(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	void read() override;

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

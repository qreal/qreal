#pragma once

#include "interpreterBase/robotModel/robotParts/display.h"

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the display emulation device for the 2D model window.
/// Configuration is perfomed immediately.
class Display : public interpreterBase::robotModel::robotParts::Display
{
	Q_OBJECT

public:
	Display(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, engine::TwoDModelEngineInterface &engine);

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

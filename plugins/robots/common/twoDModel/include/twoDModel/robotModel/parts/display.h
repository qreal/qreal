#pragma once

#include "kitBase/robotModel/robotParts/display.h"

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// An implementation of the display emulation device for the 2D model window.
/// Configuration is perfomed immediately.
class Display : public kitBase::robotModel::robotParts::Display
{
	Q_OBJECT

public:
	Display(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

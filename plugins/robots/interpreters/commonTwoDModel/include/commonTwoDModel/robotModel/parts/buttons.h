#pragma once

#include <interpreterBase/robotModel/robotParts/buttons.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// @todo Subject to generalization. Every kit has its own buttons.
class Buttons : public interpreterBase::robotModel::robotParts::Buttons
{
	Q_OBJECT

public:
	Buttons(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, engine::TwoDModelEngineInterface &engine);

	void read() override;

protected:
	void doConfiguration() override;

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

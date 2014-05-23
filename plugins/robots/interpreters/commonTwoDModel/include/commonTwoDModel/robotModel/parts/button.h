#pragma once

#include <interpreterBase/robotModel/robotParts/button.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

class Button : public interpreterBase::robotModel::robotParts::Button
{
	Q_OBJECT

public:
	Button(interpreterBase::robotModel::DeviceInfo const &info
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

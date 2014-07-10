#pragma once

#include "robotModel/parts/trikLed.h"

#include <commonTwoDModel/engine/twoDModelEngineInterface.h>
//#include <commonTwoDModel/engine/twoDModelDisplayInterface.h>



namespace trikKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {


class TwoDLed : public robotModel::parts::TrikLed
{
	Q_OBJECT

public:
	TwoDLed(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void setColor(QColor const &color) override;

private:
	//чтобы выдернуть setColor из trickDisplayWidget.h
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
};


}
}
}
}


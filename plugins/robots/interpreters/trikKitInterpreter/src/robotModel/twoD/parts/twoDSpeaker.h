#pragma once

#include "robotModel/parts/trikSpeaker.h"

#include <commonTwoDModel/engine/twoDModelEngineInterface.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {

class TwoDSpeaker : public robotModel::parts::TrikSpeaker
{
	Q_OBJECT

public:
	TwoDSpeaker(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void play(QString const &filePath) override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
};

}
}
}
}

#pragma once

#include <trikKit/robotModel/parts/trikSpeaker.h>
#include <twoDModel/engine/twoDModelEngineInterface.h>

namespace trik {
namespace robotModel {
namespace twoD {
namespace parts {

class TwoDSpeaker : public robotModel::parts::TrikSpeaker
{
	Q_OBJECT

public:
	TwoDSpeaker(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void play(const QString &filePath) override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
};

}
}
}
}

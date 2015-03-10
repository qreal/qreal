#include "twoDSpeaker.h"

using namespace trik::robotModel::twoD::parts;

const int duration = 1000;

TwoDSpeaker::TwoDSpeaker(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikSpeaker(info, port)
	, mEngine(engine)
{
}

void TwoDSpeaker::play(const QString &filePath)
{
	Q_UNUSED(filePath)
	mEngine.playSound(duration);
}

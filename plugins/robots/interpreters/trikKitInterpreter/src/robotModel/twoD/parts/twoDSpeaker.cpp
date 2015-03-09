#include "twoDSpeaker.h"

using namespace trikKitInterpreter::robotModel::twoD::parts;

const int duration = 1000;

TwoDSpeaker::TwoDSpeaker(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port
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

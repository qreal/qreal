#include "twoDSpeaker.h"

using namespace trikKitInterpreter::robotModel::twoD::parts;

int const duration = 1000;

TwoDSpeaker::TwoDSpeaker(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikSpeaker(info, port)
	, mEngine(engine)
{
}

void TwoDSpeaker::play(QString const &filePath)
{
	Q_UNUSED(filePath)
	mEngine.playSound(duration);
}

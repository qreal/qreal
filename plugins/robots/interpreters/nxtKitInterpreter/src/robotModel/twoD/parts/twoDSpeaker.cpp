#include "twoDSpeaker.h"

using namespace nxtKitInterpreter::robotModel::twoD::parts;

TwoDSpeaker::TwoDSpeaker(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::NxtSpeaker(info, port)
	, mEngine(engine)
{
}

void TwoDSpeaker::playTone(unsigned freq, unsigned time)
{
	Q_UNUSED(freq)
	mEngine.playSound(time);
}

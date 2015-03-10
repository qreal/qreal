#include "twoDSpeaker.h"

using namespace nxt::robotModel::twoD::parts;

TwoDSpeaker::TwoDSpeaker(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
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

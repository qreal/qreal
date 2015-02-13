#include "twoDMotor.h"

using namespace nxtKitInterpreter::robotModel::twoD::parts;

TwoDMotor::TwoDMotor(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: NxtMotor(info, port)
	, mEngine(engine)
{
}

void TwoDMotor::on(int speed, unsigned long degrees, bool breakMode)
{
	mEngine.setNewMotor(speed, degrees, port(), breakMode);
}

void TwoDMotor::stop(bool breakMode)
{
	on(0, 0, breakMode);
}

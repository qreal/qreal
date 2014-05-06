#include "twoDButtons.h"

using namespace trikKitInterpreter::robotModel::twoD::parts;
using namespace interpreterBase::robotModel;

TwoDButtons::TwoDButtons(DeviceInfo const &info, PortInfo const &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: TrikButtons(info, port)
	, mEngine(engine)
{
}

void TwoDButtons::read()
{
	emit response(mEngine.display()->buttonIsDown("left")
			, mEngine.display()->buttonIsDown("right")
			, mEngine.display()->buttonIsDown("up")
			, mEngine.display()->buttonIsDown("down")
			, mEngine.display()->buttonIsDown("enter")
			, mEngine.display()->buttonIsDown("escape")
			, mEngine.display()->buttonIsDown("power")
			);
}

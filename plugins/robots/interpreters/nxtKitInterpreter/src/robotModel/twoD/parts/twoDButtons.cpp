#include "twoDButtons.h"

using namespace nxtKitInterpreter::robotModel::twoD::parts;
using namespace interpreterBase::robotModel;

TwoDButtons::TwoDButtons(DeviceInfo const &info, PortInfo const &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: NxtButtons(info, port)
	, mEngine(engine)
{
}

void TwoDButtons::read()
{
	emit response(mEngine.display()->buttonIsDown("left")
			, mEngine.display()->buttonIsDown("right")
			, mEngine.display()->buttonIsDown("central")
			, mEngine.display()->buttonIsDown("bottom"));
}

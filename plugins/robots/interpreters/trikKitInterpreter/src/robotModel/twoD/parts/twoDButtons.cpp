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
	emit response(mEngine.display()->leftButtonIsDown()
			, mEngine.display()->rightButtonIsDown()
			, mEngine.display()->centralButtonIsDown()
			, mEngine.display()->bottomButtonIsDown());
}

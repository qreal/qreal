#include "commonTwoDModel/robotModel/parts/button.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"
#include "commonTwoDModel/engine/twoDModelDisplayWidget.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

Button::Button(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::Button(info, port)
	, mEngine(engine)
{
}

void Button::read()
{
	emit newData(mEngine.display()->buttonIsDown(port().name()) ? 1 : 0);
}

#include "twoDModel/robotModel/parts/button.h"

#include "twoDModel/engine/twoDModelEngineInterface.h"
#include "twoDModel/engine/twoDModelDisplayWidget.h"

using namespace twoDModel::robotModel::parts;
using namespace kitBase::robotModel;

Button::Button(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::Button(info, port)
	, mEngine(engine)
{
}

void Button::read()
{
	emit newData(mEngine.display()->buttonIsDown(port().name()) ? 1 : 0);
}

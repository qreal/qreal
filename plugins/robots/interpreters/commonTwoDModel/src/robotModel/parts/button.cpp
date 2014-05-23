#include "commonTwoDModel/robotModel/parts/button.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

Button::Button(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::Button(info, port)
	, mEngine(engine)
{
}

void Button::read()
{
	emit newData(mEngine.display()->buttonIsDown(port().name()) ? 1 : 0);
}

void Button::doConfiguration()
{
	configurationCompleted(true);
}

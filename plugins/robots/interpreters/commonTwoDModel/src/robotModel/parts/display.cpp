#include "commonTwoDModel/robotModel/parts/display.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

Display::Display(DeviceInfo const &info
		, PortInfo const &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::Display(info, port)
	, mEngine(engine)
{
}

void Display::doConfiguration()
{
	configurationCompleted(true);
}

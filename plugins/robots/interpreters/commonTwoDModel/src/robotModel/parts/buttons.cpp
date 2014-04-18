#include "commonTwoDModel/robotModel/parts/buttons.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

Buttons::Buttons(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::Buttons(info, port)
	, mEngine(engine)
{
}

void Buttons::read()
{
	/// @todo Implement.
}

void Buttons::doConfiguration()
{
	configurationCompleted(true);
}

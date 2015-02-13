#include "commonTwoDModel/robotModel/parts/gyroscope.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

Gyroscope::Gyroscope(const DeviceInfo &info
		, const PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::GyroscopeSensor(info, port)
	, mEngine(engine)
{
}

void Gyroscope::read()
{
	/// @todo: Emulate gyroscope in 2D model
	emit newData(0);
}

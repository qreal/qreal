#include "twoDModel/robotModel/parts/touchSensor.h"

#include "twoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace kitBase::robotModel;

TouchSensor::TouchSensor(const DeviceInfo &info
		, const PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::TouchSensor(info, port)
	, mEngine(engine)
{
}

void TouchSensor::read()
{
	emit newData(mEngine.readTouchSensor(port()));
}

#include "commonTwoDModel/robotModel/parts/touchSensor.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

TouchSensor::TouchSensor(DeviceInfo const &info
		, PortInfo const &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::TouchSensor(info, port)
	, mEngine(engine)
{
}

void TouchSensor::read()
{
	emit newData(mEngine.readTouchSensor(deviceInfo(), port()));
}

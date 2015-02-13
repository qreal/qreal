#include "commonTwoDModel/robotModel/parts/lightSensor.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

LightSensor::LightSensor(const DeviceInfo &info
		, const PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::LightSensor(info, port)
	, mEngine(engine)
{
}

void LightSensor::read()
{
	emit newData(mEngine.readLightSensor(port()));
}

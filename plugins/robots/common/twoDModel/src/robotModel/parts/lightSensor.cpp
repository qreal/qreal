#include "twoDModel/robotModel/parts/lightSensor.h"

#include "twoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace kitBase::robotModel;

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

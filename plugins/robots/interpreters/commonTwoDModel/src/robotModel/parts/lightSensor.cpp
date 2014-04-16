#include "commonTwoDModel/robotModel/parts/lightSensor.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

LightSensor::LightSensor(DeviceInfo const &info
		, PortInfo const &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::LightSensor(info, port)
	, mEngine(engine)
{
}

void LightSensor::read()
{
	/// @todo Implement.
//	emit newData(mEngine.readLightSensor(port()));
}

void LightSensor::doConfiguration()
{
}

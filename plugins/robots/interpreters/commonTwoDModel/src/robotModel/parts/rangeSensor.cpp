#include "commonTwoDModel/robotModel/parts/rangeSensor.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

RangeSensor::RangeSensor(const DeviceInfo &info, const PortInfo &port, engine::TwoDModelEngineInterface &engine)
	: robotParts::RangeSensor(info, port)
	, mEngine(engine)
{
}

void RangeSensor::read()
{
	emit newData(mEngine.readSonarSensor(port()));
}

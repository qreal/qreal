#include "commonTwoDModel/robotModel/parts/rangeSensor.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

RangeSensor::RangeSensor(DeviceInfo const &info, PortInfo const &port, engine::TwoDModelEngineInterface &engine)
	: robotParts::RangeSensor(info, port)
	, mEngine(engine)
{
}

void RangeSensor::read()
{
	emit newData(mEngine.readSonarSensor(port()));
}

#include "commonTwoDModel/robotModel/parts/accelerometer.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

Accelerometer::Accelerometer(const DeviceInfo &info
		, const PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::AccelerometerSensor(info, port)
	, mEngine(engine)
{
}

void Accelerometer::read()
{
	/// @todo: Emulate accelerometer in 2D model
	emit newData(0);
}

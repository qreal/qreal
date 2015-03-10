#include "twoDModel/robotModel/parts/accelerometer.h"

#include "twoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace kitBase::robotModel;

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

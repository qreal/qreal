#include "commonTwoDModel/robotModel/parts/colorSensor.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

ColorSensor::ColorSensor(DeviceInfo const &info
		, PortInfo const &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::ColorSensor(info, port)
	, mEngine(engine)
{
}

void ColorSensor::read()
{
	/// @todo Implement.
//	emit newData(mEngine.readColorSensor(port()));
}

void ColorSensor::doConfiguration()
{
}

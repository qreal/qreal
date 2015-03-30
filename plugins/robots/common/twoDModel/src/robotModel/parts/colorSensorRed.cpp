#include "twoDModel/robotModel/parts/colorSensorRed.h"

#include "twoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace kitBase::robotModel;

ColorSensorRed::ColorSensorRed(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: kitBase::robotModel::robotParts::ColorSensorRed(info, port)
	, mEngine(engine)
{
}

void ColorSensorRed::read()
{
	emit newData(mEngine.readColorSensor(port()));
}

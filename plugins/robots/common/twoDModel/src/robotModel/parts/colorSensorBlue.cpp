#include "twoDModel/robotModel/parts/colorSensorBlue.h"

#include "twoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace kitBase::robotModel;

ColorSensorBlue::ColorSensorBlue(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: kitBase::robotModel::robotParts::ColorSensorBlue(info, port)
	, mEngine(engine)
{
}

void ColorSensorBlue::read()
{
	emit newData(mEngine.readColorSensor(port()));
}

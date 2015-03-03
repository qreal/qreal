#include "commonTwoDModel/robotModel/parts/colorSensorBlue.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

ColorSensorBlue::ColorSensorBlue(const interpreterBase::robotModel::DeviceInfo &info
		, const interpreterBase::robotModel::PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: interpreterBase::robotModel::robotParts::ColorSensorBlue(info, port)
	, mEngine(engine)
{
}

void ColorSensorBlue::read()
{
	emit newData(mEngine.readColorSensor(port()));
}

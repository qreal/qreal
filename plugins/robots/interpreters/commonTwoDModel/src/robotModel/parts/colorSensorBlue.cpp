#include "commonTwoDModel/robotModel/parts/colorSensorBlue.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

ColorSensorBlue::ColorSensorBlue(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, engine::TwoDModelEngineInterface &engine)
	: interpreterBase::robotModel::robotParts::ColorSensorBlue(info, port)
	, mEngine(engine)
{
}

void ColorSensorBlue::read()
{
	emit newData(mEngine.readColorSensor(port()));
}
